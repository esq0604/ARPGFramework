// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionFramework/UI/EscMenuWidget.h"
#include "ActionFramework/UI/EscPresenter.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "Components/Button.h"
#include "EscMenuWidget.h"

void UEscMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventoryButton)
    {
        InventoryButton->OnClicked.AddDynamic(this, &UEscMenuWidget::HandleInventoryButtonClicked);
    }

    if (EquipmentButton)
    {
        EquipmentButton->OnClicked.AddDynamic(this, &UEscMenuWidget::HandleEquipmentButtonClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UEscMenuWidget::HandleExitButtonClicked);
    }
}

void UEscMenuWidget::NativeDestruct()
{
    if (InventoryButton)
    {
        InventoryButton->OnClicked.RemoveDynamic(this, &UEscMenuWidget::HandleInventoryButtonClicked);
    }

    if (EquipmentButton)
    {
        EquipmentButton->OnClicked.RemoveDynamic(this, &UEscMenuWidget::HandleEquipmentButtonClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.RemoveDynamic(this, &UEscMenuWidget::HandleExitButtonClicked);
    }
}

void UEscMenuWidget::WidgetPresenterSet()
{
   // UEscPresenter* EscPresenter = Cast<UEscPresenter>(GetPresenter());
}

void UEscMenuWidget::HandleInventoryButtonClicked()
{
    UEscPresenter* EscPresenter = Cast<UEscPresenter>(GetPresenter());
    SetVisibility(ESlateVisibility::Collapsed);
    //EscPresenter->
	//OnMenuButtonClicked.Broadcast(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.OepnInventoryWidget")));

}

void UEscMenuWidget::HandleEquipmentButtonClicked()
{
	OnMenuButtonClicked.Broadcast(ARPGGameplayTags::GameplayEvent_OpenEquipment);
    SetVisibility(ESlateVisibility::Collapsed);
}

void UEscMenuWidget::HandleExitButtonClicked()
{
	OnMenuButtonClicked.Broadcast(ARPGGameplayTags::GameplayEvent_ExitGame);
    SetVisibility(ESlateVisibility::Collapsed);
}
