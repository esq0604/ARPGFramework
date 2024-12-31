// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionFramework/UI/EscMenuWidget.h"
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

void UEscMenuWidget::HandleInventoryButtonClicked()
{
	OnMenuButtonClicked.Broadcast(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.OepnInventoryWidget")));

}

void UEscMenuWidget::HandleEquipmentButtonClicked()
{
	OnMenuButtonClicked.Broadcast(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.OpenEquipmentWidget")));

}

void UEscMenuWidget::HandleExitButtonClicked()
{
	OnMenuButtonClicked.Broadcast(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.ExitGame")));

}
