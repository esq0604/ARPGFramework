// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/OverlayWidget.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "ActionFramework/UI/GaugeBar.h"
#include "Components/TextBlock.h"


void UOverlayWidget::ShowBossUI()
{
	BossHealthGaugeVisibilityChange(true);
	BossName->SetVisibility(ESlateVisibility::Visible);
}

void UOverlayWidget::WidgetPresenterSet()
{
	if (UOverlayPresenter* OverlayPresenter = Cast<UOverlayPresenter>(GetPresenter()))
	{
		//Gauge Percent 바인딩
		PlayerHealthGauge->BindPercentDelegate(OverlayPresenter->OnPlayerHealthPercentChange);
		PlayerPostureGauge->BindPercentDelegate(OverlayPresenter->OnPlayerPosturePercentChange);

		BossHealthGauge->BindPercentDelegate(OverlayPresenter->OnBossHealthPercentChange);
		BossPostureGauge->BindPercentDelegate(OverlayPresenter->OnBossPosturePercentChange);

		// 가시성 제어 연결
		OverlayPresenter->OnPlayerPosturePercentChange.AddDynamic(this, &UOverlayWidget::PlayerPostureGaugeVisibilityChange);
		OverlayPresenter->OnBossHealthPercentChange.AddDynamic(this, &UOverlayWidget::BossHealthGaugeVisibilityChange);
		OverlayPresenter->OnBossPosturePercentChange.AddDynamic(this, &UOverlayWidget::BossPostureGaugeVisibilityChange);
		//OverlayPresenter->OnBossNameChange.
	}


}

void UOverlayWidget::PlayerPostureGaugeVisibilityChange(float Percent)
{
	if (Percent > 0.f)
	{
		PlayerPostureGauge->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().ClearTimer(PlayerPostureHideTimerHandle);
		bPlayerPostureHidden = false;
	}
	else if (!bPlayerPostureHidden)
	{
		bPlayerPostureHidden = true;
		GetWorld()->GetTimerManager().SetTimer(PlayerPostureHideTimerHandle, this, &UOverlayWidget::HidePlayerPostureGauge, PostureAutoHideDelay, false);
	}
}

void UOverlayWidget::BossHealthGaugeVisibilityChange(float Percent)
{
	Percent ? BossHealthGauge->SetVisibility(ESlateVisibility::Visible) : BossHealthGauge->SetVisibility(ESlateVisibility::Hidden);
}

void UOverlayWidget::BossPostureGaugeVisibilityChange(float Percent)
{
	if (Percent > 0.f)
	{
		BossPostureGauge->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().ClearTimer(BossPostureHideTimerHandle);
		bBossPostureHidden = false;
	}
	else if (!bBossPostureHidden)
	{
		bBossPostureHidden = true;
		GetWorld()->GetTimerManager().SetTimer(BossPostureHideTimerHandle, this, &UOverlayWidget::HideBossPostureGauge, PostureAutoHideDelay, false);
	}
}

void UOverlayWidget::HidePlayerPostureGauge()
{
	PlayerPostureGauge->SetVisibility(ESlateVisibility::Hidden);

}

void UOverlayWidget::HideBossPostureGauge()
{
	BossPostureGauge->SetVisibility(ESlateVisibility::Hidden);
}

