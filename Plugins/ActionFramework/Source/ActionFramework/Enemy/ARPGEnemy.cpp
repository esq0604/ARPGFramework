// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemy.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Components/TargetingComponent.h"
#include "ActionFramework/Enemy/ARPGAIController.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AARPGEnemy::AARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UARPGAttributeSet>("AttributeSet");
	ASC->AddAttributeSetSubobject<UAttributeSet>(AttributeSet);

}

void AARPGEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARPGAIController = Cast<AARPGAIController>(NewController);
	ARPGAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	ARPGAIController->RunBehaviorTree(BehaviorTree);

	ARPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
}

AActor* AARPGEnemy::GetEquippedWeapon_Implementation()
{
	return WeaponInstance;
}

UAbilitySystemComponent* AARPGEnemy::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AARPGEnemy::BeginPlay()
{
	Super::BeginPlay();

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

	ASC->RegisterGameplayTagEvent(ARPGGameplayTags::GameplayEvent_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&ThisClass::HitReactTagChanged
	);
}

void AARPGEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (ASC)
	{
		const UARPGAttributeSet* Attribute = ASC->GetSet<UARPGAttributeSet>();
		ASC->InitAbilityActorInfo(this, this);
		if (Attribute)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute()).AddUObject(this, &AARPGEnemy::OnHealthChange);
		}
	}
}

void AARPGEnemy::Dead_Implementation()
{

}

// Called every frame
void AARPGEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARPGEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AARPGEnemy::OnHealthChange(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.f)
	{
		Dead();
	}
	else if (Data.NewValue <= Data.OldValue)
	{
		// 1��. HitReactionComponent->ExecuteHitReaction(���ݹ����±װ� �Ű������� �����մϴ�.);
		// 2�� ASC->TryActivateAbility�� ���� HitReactionAbility�� �����ŵ�ϴ�.(������ ���⼭�� HitReactionComponent�� ���� ó���Ұ��̱� ������ �±װ� �ʿ��մϴ�.)

		//3�� ��������� �� ����?
	}
	
}

void AARPGEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount)
{
	bHitReacting = NewTagCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

