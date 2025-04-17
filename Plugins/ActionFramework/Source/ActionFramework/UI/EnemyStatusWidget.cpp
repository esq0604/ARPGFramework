// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EnemyStatusWidget.h"
#include "ActionFramework/UI/GaugeBar.h"
#include "ActionFramework/Enemy/ARPGEnemy.h"

void UEnemyStatusWidget::ChangeVisibility()
{
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);

	}

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
	UE_LOG(LogTemp, Warning, TEXT("OnEnemyHealthChanged, Widget"));
	
	SetBarPercent(HealthBar, Health, MaxHealth);
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
		HealthBar->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().ClearTimer(StatusBarHideTimerHandle);
		bBossPostureHidden = false;
	}
	else if (!bBossPostureHidden)
	{
		bBossPostureHidden = true;
		GetWorld()->GetTimerManager().SetTimer(StatusBarHideTimerHandle, this, &UEnemyStatusWidget::StatusBarGaugeVisibilityChange, PostureAutoHideDelay, false);
	}
}

void UEnemyStatusWidget::PostureGaugeVisibilityChange(float NewVal)
{
}

void UEnemyStatusWidget::SetBarPercent(UGaugeBar* Widget, float Val, float MaxVal)
{
	Widget->SetBarPercent(Val/MaxVal);
}
