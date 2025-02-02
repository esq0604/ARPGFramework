// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/OverlayWidget.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "ActionFramework/UI/GaugeBar.h"

void UOverlayWidget::WidgetPresenterSet()
{
	HPGauge->SetPresenter(GetPresenter());
}
