// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ARPGUserWidget.h"

void UARPGUserWidget::SetPresenter(UARPGPresenter* NewPresenter)
{
	Presenter = NewPresenter;
	WidgetPresenterSet();
}

void UARPGUserWidget::WidgetPresenterSet()
{

}
