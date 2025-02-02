// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/SlotWrapperWidget.h"
#include "ActionFramework/UI/EquipmentPresenter.h"
#include "ActionFramework/UI/Slot.h"
#include "Components/UniformGridPanel.h"
void USlotWrapperWidget::OnSlotClicked(USlot* ClickedSlot)
{
	
}
void USlotWrapperWidget::WidgetPresenterSet()
{
	//UEquipmentPresenter* EquipPresenter = Cast<UEquipmentPresenter>(GetPresenter());
	//
	//TArray<UWidget*> WeaponSlots = WeaponPanel->GetAllChildren();
	//for (const auto& Slot : WeaponSlots)
	//{
	//	USlot* SlotWidget = Cast<USlot>(Slot);
	//	SlotWidget->OnSlotClicked.AddDynamic(this, &USlotWrapperWidget::OnSlotClicked);
	//}
}
