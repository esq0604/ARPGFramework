// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/UI/SlotViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USlot::NativeConstruct()
{
	Super::NativeConstruct();

	AmountText->SetVisibility(ESlateVisibility::Collapsed);
	//BackgroundIcon->SetBrushFromTexture(BackgroundIconTexture);
}

void USlot::ClearItemInfo()
{
	Icon->SetBrushFromTexture(nullptr);
	AmountText->SetText(FText::GetEmpty());
	//ItemName->SetText(FText::GetEmpty());
}

void USlot::UpdateItemInfo(const FSlotDisplayInfo& ItemInfo)
{
	//CurrentItemInfo = ItemInfo;  // 데이터 저장

	// UI 업데이트
	Icon->SetBrushFromTexture(ItemInfo.Icon);
	AmountText->SetText(FText::AsNumber(ItemInfo.Count));
	//ItemNameText->SetText(ItemInfo.ItemName);
}

FReply USlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClicked.Broadcast(this);  // 클릭 이벤트 브로드캐스트
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void USlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("MouseEnter"));
	OnSlotMouseEnter.Broadcast(this);
}

void USlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("MouseLeave"));
	OnSlotMouseEnter.Broadcast(this);
}
