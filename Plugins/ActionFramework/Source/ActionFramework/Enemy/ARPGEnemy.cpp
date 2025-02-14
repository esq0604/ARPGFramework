// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemy.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Components/TargetingComponent.h"
#include "ActionFramework/Enemy/ARPGAIController.h"
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
		// 1번. HitReactionComponent->ExecuteHitReaction(공격방향태그가 매개변수로 들어가야합니다.);
		// 2번 ASC->TryActivateAbility를 통해 HitReactionAbility를 실행시킵니다.(하지만 여기서도 HitReactionComponent를 통해 처리할것이기 떄문에 태그가 필요합니다.)

		//3번 괜찮은방법 뭐 없나?
	}
	
}

