// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EnemyStatusWidget.h"
#include "ActionFramework/UI/GaugeBar.h"
#include "ActionFramework/Enemy/ARPGEnemy.h"
#include "ActionFramework/ARPGGameplayTags.h"

UEnemyStatusWidget::UEnemyStatusWidget()
{
}

void UEnemyStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetTargetingState(false);
}

void UEnemyStatusWidget::ChangeVisibility()
{

}

void UEnemyStatusWidget::WidgetPresenterSet()
{
	AARPGEnemy* BaseEnemy = Cast<AARPGEnemy>(GetPresenter());

	if (BaseEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemtStatus Widget Bind With Enemy Status"));
		BaseEnemy->OnHealthChanged.AddDynamic(this, &UEnemyStatusWidget::OnEnemyHealthChanged);
		BaseEnemy->OnPostureChanged.AddDynamic(this, &UEnemyStatusWidget::OnEnemyPostureChanged);
		BaseEnemy->OnMaxHealthChanged.AddDynamic(this, &UEnemyStatusWidget::OnEnemyMaxHealthChanged);
		BaseEnemy->OnMaxPostureChanged.AddDynamic(this, &UEnemyStatusWidget::OnEnemyMaxPostureChanged);

		//가시성 제어
		BaseEnemy->OnHealthChanged.AddDynamic(this, &UEnemyStatusWidget::StatusBarGaugeVisibilityChange);
		BaseEnemy->OnPostureChanged.AddDynamic(this, &UEnemyStatusWidget::StatusBarGaugeVisibilityChange);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemtStatus Widget Bind With Enemy Status not Cast"));

	}
}

void UEnemyStatusWidget::OnEnemyHealthChanged(float NewVal)
{
	Health = NewVal;
	SetBarPercent(HealthBar, Health, MaxHealth);
	NotifyDamaged();
}

void UEnemyStatusWidget::OnEnemyPostureChanged(float NewVal)
{
	Posture = NewVal;
	SetBarPercent(PostureBar, Posture, MaxPosture);
}

void UEnemyStatusWidget::OnEnemyMaxHealthChanged(float NewVal)
{
	MaxHealth = NewVal;
	SetBarPercent(HealthBar, Health, MaxHealth);
}

void UEnemyStatusWidget::OnEnemyMaxPostureChanged(float NewVal)
{
	MaxPosture = NewVal;
	SetBarPercent(PostureBar, Posture, MaxPosture);
}

void UEnemyStatusWidget::StatusBarGaugeVisibilityChange(float NewVal)
{
	if (NewVal > 0.f)
	{
		NotifyDamaged();
	}
}

void UEnemyStatusWidget::HidePostureStatusBar()
{
}

void UEnemyStatusWidget::HideHealthBar()
{
}



void UEnemyStatusWidget::SetBarPercent(UGaugeBar* Widget, float Val, float MaxVal)
{
	Widget->SetBarPercent(Val/MaxVal);
}

void UEnemyStatusWidget::SetStatusBarState(const FGameplayTag& NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		UpdateStatusBarVisibility();
	}
}

void UEnemyStatusWidget::UpdateStatusBarVisibility()
{
	const bool bVisible = !CurrentState.MatchesTagExact(ARPGGameplayTags::StatusBar_State_None);
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	UE_LOG(LogTemp, Warning, TEXT("StatusBar visibility updated to: %s (%s)"),
		bVisible ? TEXT("Visible") : TEXT("Hidden"),
		*CurrentState.ToString());
}

void UEnemyStatusWidget::SetTargetingState(bool bTargeting)
{
	if (bTargeting)
	{
		GetWorld()->GetTimerManager().ClearTimer(StatusBarHideTimerHandle);
		SetStatusBarState(ARPGGameplayTags::StatusBar_State_Targeted);
	}
	else
	{
		SetStatusBarState(ARPGGameplayTags::StatusBar_State_None);
	}
}

void UEnemyStatusWidget::NotifyDamaged()
{
	// 타겟 상태면 유지
	if (CurrentState == ARPGGameplayTags::StatusBar_State_Targeted) return;

	SetStatusBarState(ARPGGameplayTags::StatusBar_State_DamageResponse);
	GetWorld()->GetTimerManager().SetTimer(StatusBarHideTimerHandle, this, &UEnemyStatusWidget::TryHideStatusBar, StatusBarAutoHideDelay, false);
}

void UEnemyStatusWidget::TryHideStatusBar()
{
	if (CurrentState == ARPGGameplayTags::StatusBar_State_DamageResponse)
	{
		SetStatusBarState(ARPGGameplayTags::StatusBar_State_None);
	}
}
