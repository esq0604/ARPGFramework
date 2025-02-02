// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/GaugeBar.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "Components/ProgressBar.h"

void UGaugeBar::WidgetPresenterSet()
{
	UOverlayPresenter* OverlayPresenter = Cast<UOverlayPresenter>(GetPresenter());
	if (OverlayPresenter)
	{
		OverlayPresenter->OnHealthPercentChange.AddDynamic(this, &UGaugeBar::UpdateGauge);
	}

}

void UGaugeBar::UpdateGauge(float Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("Uddate Gauge : %f"), Percent);
	ProgressBar->SetPercent(Percent);
}
