// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/GaugeBar.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "Components/ProgressBar.h"


void UGaugeBar::WidgetPresenterSet()
{
	
}

void UGaugeBar::SetBarPercent(float NewVal)
{
	ProgressBar->SetPercent(NewVal);
}


void UGaugeBar::BindPercentDelegate(FOnPercentChangeDelegate& InDelegate)
{
	if(!InDelegate.IsBound())
		InDelegate.AddDynamic(this, &UGaugeBar::SetBarPercent);
}
