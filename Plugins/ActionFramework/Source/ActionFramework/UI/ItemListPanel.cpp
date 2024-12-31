// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ItemListPanel.h"
#include "ActionFramework/UI/Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UItemListPanel::NativeConstruct()
{
}

void UItemListPanel::InitializeSlot(int32 TotalSlots)
{
    if (!SlotWidgetClass || !UniformGridPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("SlotWidgetClass or UniformGridPanel is null."));
        return;
    }

    // 기존 슬롯 제거 및 풀 초기화
    SlotPool.Empty();
    UniformGridPanel->ClearChildren();

    for (int32 i = 0; i < TotalSlots; ++i)
    {
        USlot* NewSlotWidget = CreateWidget<USlot>(this, SlotWidgetClass);
        if (NewSlotWidget)
        {
            NewSlotWidget->OnSlotMouseEnter.AddDynamic(this, &UItemListPanel::OnSlotMouseEntered);
            NewSlotWidget->OnSlotClicked.AddDynamic(this, &UItemListPanel::OnSlotClicked);
            SlotPool.Add(NewSlotWidget);
            UniformGridPanel->AddChildToUniformGrid(NewSlotWidget,(i / NumColumns),(i % NumColumns));
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Initialized %d slots."), TotalSlots);
}

void UItemListPanel::ClearSlots()
{
    for (UUserWidget* SlotWidget : SlotPool)
    {
        if (USlot* PanelSlot = Cast<USlot>(SlotWidget))
        {
            PanelSlot->ClearItemInfo();  // 슬롯 데이터 초기화
        }
    }
}

void UItemListPanel::UpdateSlot(int32 SlotIndex, const FSlotDisplayInfo& ItemInfo)
{
    if (SlotPool.IsValidIndex(SlotIndex))
    {
        UUserWidget* SlotWidget = SlotPool[SlotIndex];
        if (USlot* PanelSlot = Cast<USlot>(SlotWidget))
        {
            PanelSlot->UpdateItemInfo(ItemInfo);  // 슬롯 데이터 갱신
        }
    }
}

void UItemListPanel::OnSlotClicked(USlot* ClickedSlot)
{
    if (!ClickedSlot)
    {
        UE_LOG(LogTemp, Warning, TEXT("ClickedSlot is null."));
        return;
    }

    // SlotPool에서 ClickedSlot의 인덱스를 검색
    int32 SlotIndex = SlotPool.IndexOfByKey(ClickedSlot);

    if (SlotIndex != INDEX_NONE)  // 유효한 인덱스 확인
    {
        UE_LOG(LogTemp, Log, TEXT("Clicked Slot Index: %d"), SlotIndex);

        // 슬롯 클릭 이벤트를 인덱스와 함께 브로드캐스트
        OnItemListSlotClicked.Broadcast(SlotIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ClickedSlot not found in SlotPool."));
    }
}

void UItemListPanel::OnSlotMouseEntered(USlot* EnteredSlot)
{
    if (!EnteredSlot)
    {

        int32 SlotIndex = SlotPool.IndexOfByKey(EnteredSlot);

        //OnItemListSlotEntered.BroadCast(SlotIndex);
        return;
    }
}
