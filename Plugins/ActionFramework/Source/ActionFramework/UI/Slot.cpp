// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/UI/EquipmentPresenter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USlot::NativeConstruct()
{
	Super::NativeConstruct();

	AmountText->SetVisibility(ESlateVisibility::Collapsed);
	//BackgroundIcon->SetBrushFromTexture(BackgroundIconTexture);
}

void USlot::ClearSlot()
{
	Icon->SetBrushFromTexture(nullptr);
	//AmountText->SetText(FText::GetEmpty());
	//ItemName->SetText(FText::GetEmpty());
}

void USlot::UpdateSlot(const FSlotDisplayInfo& ItemInfo)
{
	//CurrentItemInfo = ItemInfo;  // ������ ����

	// UI ������Ʈ
	Icon->SetBrushFromTexture(ItemInfo.Icon);
	AmountText->SetText(FText::AsNumber(ItemInfo.Count));
	StoreItemText = ItemInfo.Name;

	if (ItemInfo.bIsEquipped)
	{
		EquipCheck->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EquipCheck->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlot::SetSlotType(FGameplayTag TypeTag)
{
	SlotType = TypeTag;
}

FReply USlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClicked.Broadcast(this);  // Ŭ�� �̺�Ʈ ��ε�ĳ��Ʈ
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void USlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnSlotMouseEnter.Broadcast(this);
}

void USlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	//Super::NativeOnMouseLeave(InMouseEvent);
}
