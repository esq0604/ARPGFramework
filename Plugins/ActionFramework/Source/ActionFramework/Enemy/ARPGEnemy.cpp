// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGEnemy.h"
#include "ActionFramework/AbilitySystem/ARPGAbilitySystemComponent.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Components/TargetingComponent.h"
#include "ActionFramework/Enemy/ARPGAIController.h"
#include "ActionFramework/ARPGGameplayTags.h"

#include "Components/WidgetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AARPGEnemy::AARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	TargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidgetComponent"));
	TargetWidgetComponent->SetupAttachment(RootComponent);
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
	TargetingComponent->SetupAttachment(RootComponent);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UARPGAttributeSet>("AttributeSet");
	ASC->AddAttributeSetSubobject<UAttributeSet>(AttributeSet.Get());
}

void AARPGEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (ASC)
	{
		const UARPGAttributeSet* Attribute = ASC->GetSet<UARPGAttributeSet>();
		ASC->InitAbilityActorInfo(this, this);
		AddCharacterAbilities();
		InitDefaultAttribute();
	}


	ARPGAIController = Cast<AARPGAIController>(NewController);
	if (ARPGAIController)
	{
		UBlackboardComponent* BBComp = ARPGAIController->GetBlackboardComponent();
		if (BBComp && BehaviorTree)
		{
			BBComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
			ARPGAIController->RunBehaviorTree(BehaviorTree);
			ARPGAIController->GetBlackboardComponent()->SetValueAsObject(FName("SelfActor"), this);
			ARPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
			ARPGAIController->GetBlackboardComponent()->SetValueAsObject(FName("PatternDataTable"), PatternDataTable);
		}
	}
}

AActor* AARPGEnemy::GetEquippedWeapon_Implementation()
{
	return WeaponInstance;
}

void AARPGEnemy::ToggleTargeting(bool bEnable)
{
	if (TargetWidgetComponent)
	{
		TargetWidgetComponent->SetVisibility(bEnable); // 락온 마커 표시 (공통)
	}
}

UAbilitySystemComponent* AARPGEnemy::GetAbilitySystemComponent() const
{
	return ASC;
}

void AARPGEnemy::SetCombatTarget(AActor* NewCombatTarget)
{
	CombatTarget = NewCombatTarget;
}

AActor* AARPGEnemy::GetCombatTarget()
{
	return CombatTarget;
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

	}

	ASC->RegisterGameplayTagEvent(ARPGGameplayTags::GameplayEvent_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&ThisClass::HitReactTagChanged
	);

	if(TargetWidgetComponent)
	{
		TargetWidgetComponent->InitWidget();
		TargetWidgetComponent->SetVisibility(false);
	}
	//전투 상태에 돌입했을 때의 대한 이벤트를 만들도록 합니다.

	//전투 상태에 돌입했다면 체력을 보여지도록 합니다
		//1. 부모 상속을 이용해 보스, 노멀 몬스터의 체력을 보여지는걸 다르게 구현한다 - (현재 보스는 플레이어의 hud에 보이도록, 노멀은 자기 위에 보여지도록)
		//2. 인터페이스를 이용해.


}

void AARPGEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

	}
	
}

void AARPGEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount)
{
	bHitReacting = NewTagCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AARPGEnemy::AddCharacterAbilities()
{
	UARPGAbilitySystemComponent* ARPGASC = CastChecked<UARPGAbilitySystemComponent>(ASC);
	ARPGASC->AddCharacterAbilities(StartAbilities);

}

void AARPGEnemy::InitDefaultAttribute()
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultAttributeEffect);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	ContextHandle.AddInstigator(this,this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	//ASC->ApplyGameplayEffectToSelf(DefaultAttributeEffect->GetDefaultObject,)
	//ASC->InitStats()
	UE_LOG(LogTemp, Warning, TEXT("GameplayEffectToTarget - InitEnemyDefault Attr %s"), *GetName());
}	

