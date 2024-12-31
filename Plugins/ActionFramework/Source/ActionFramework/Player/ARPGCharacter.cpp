// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCharacter.h"

#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/Camerra/ARPGSpringArmComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "ActionFramework/Components/TargetingComponent.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AARPGCharacter::AARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComp = CreateDefaultSubobject<UARPGSpringArmComponent>(TEXT("ARPGSpringArm"));
	
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	

	//SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryCoponent"));

	BaseEyeHeight = 74.0f;

}

void AARPGCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer =  GetAbilitySystemComponent()->GetOwnedGameplayTags();
}


// Called when the game starts or when spawned
void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	IAbilitySystemInterface* ASCInterface = nullptr;
	if (GetPlayerState())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerState %s"), *GetPlayerState()->GetName());
		ASCInterface = Cast<IAbilitySystemInterface>(GetPlayerState());

	}
	if (ASCInterface != nullptr)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		if (ASC)
		{
			ASC->InitAbilityActorInfo(this, this);
			
			if (GetMesh())
			{	
				if (GetMesh()->GetAnimInstance())
				{
					UARPGAnimInstance* AnimInstance = Cast<UARPGAnimInstance>(GetMesh()->GetAnimInstance());
					if (AnimInstance)
					{
						AnimInstance->InitializeWithAbilitySystem(ASC);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("GetMesh -> GetAnimInstance nullptr"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GetMesh nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player ASC nullptr"));
		}
	}
}

void AARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (SpringArmComp)
	{
		if (SpringArmComp->IsCameraLockedToTarget())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Target Set , NewRot"));

			// Vector from player to target
			FVector TargetVect = SpringArmComp->CameraTarget->GetComponentLocation() - SpringArmComp->GetComponentLocation();
			FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
			FRotator CurrentRot = GetControlRotation();
			FRotator InterpRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, SpringArmComp->LockonControlRotationRate);

			FRotator NewRot = FRotator(GetActorRotation().Pitch, InterpRot.Yaw, GetActorRotation().Roll);
			// Update control rotation to face target
			GetController()->SetControlRotation(NewRot);
			SetActorRotation(NewRot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SrpingArm nullptr"));
	}
}

AActor* AARPGCharacter::GetEquippedWeapon_Implementation()
{
	UItemBaseDataAsset* Data = InventoryComponent->GetCurrentEquipWeaponData();
	if (Data)
	{
		AActor* Item = Data->GetSpawnedActor();
		if (Item)
		{
			return Item;
		}
	}
	return nullptr;
}

void AARPGCharacter::ToggleTargeting(bool bEnable)
{

}

UAbilitySystemComponent* AARPGCharacter::GetAbilitySystemComponent() const
{
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (ASCInterface != nullptr)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		if (ASC)
		{
			return ASC;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player ASC nullptr"));
	return nullptr;
}

