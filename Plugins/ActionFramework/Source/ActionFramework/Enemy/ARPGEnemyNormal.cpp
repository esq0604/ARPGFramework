// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Enemy/ARPGEnemyNormal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/UI/EnemyStatusWidget.h"
#include "Components/WidgetComponent.h"

AARPGEnemyNormal::AARPGEnemyNormal()
{
	EnemyStatusBar = CreateDefaultSubobject<UWidgetComponent>("HealthPostureBar");
	EnemyStatusBar->SetupAttachment(GetRootComponent());
}

UAbilitySystemComponent* AARPGEnemyNormal::GetAbilitySystemComponent() const
{
	return ASC;
}

void AARPGEnemyNormal::ToggleTargeting(bool bEnable)
{
	// EnemyStatusBar가시성 설정.
	Super::ToggleTargeting(bEnable); // 공통 락온 마커 표시

	if (UEnemyStatusWidget* StatusWidget = Cast<UEnemyStatusWidget>(EnemyStatusBar->GetUserWidgetObject()))
	{
		StatusWidget->SetTargetingState(bEnable); // 체력 바 표시
	}
}



void AARPGEnemyNormal::BeginPlay()
{
	Super::BeginPlay();

}


void AARPGEnemyNormal::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	EnemyStatusBar->InitWidget();
	UUserWidget* Widget = EnemyStatusBar->GetUserWidgetObject();
	if (Widget)
	{
		if (UARPGUserWidget* ARPGUserWidget = Cast<UARPGUserWidget>(Widget))
		{
			ARPGUserWidget->SetPresenter(this);
		}
	}

	if (const UARPGAttributeSet* AS = Cast<UARPGAttributeSet>(AttributeSet))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change Normal Enemy Health %f"), Data.NewValue);

			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetPostureAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnPostureChanged.Broadcast(Data.NewValue);
		}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxPostureAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxPostureChanged.Broadcast(Data.NewValue);
		}
		);
	}

	//if (Widget)
	//	Widget->SetVisibility(ESlateVisibility::Hidden);
}
