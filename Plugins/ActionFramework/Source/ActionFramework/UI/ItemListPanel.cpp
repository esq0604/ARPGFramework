// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ItemListPanel.h"
#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/UI/EquipmentPresenterMediator.h"
#include "ItemListPaenlPresenter.h"
#include "EquipmentPresenter.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UItemListPanel::NativeConstruct()
{
}

void UItemListPanel::InitializeSlot(uint8 Size)
{
    if (!SlotWidgetClass || !UniformGridPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("SlotWidgetClass or UniformGridPanel is null."));
        return;
    }

    // 기존 슬롯 제거 및 풀 초기화
    SlotPool.Empty();
    UniformGridPanel->ClearChildren();

    for (uint8 i = 0; i < Size; i++)
    {

        USlot* NewSlotWidget = CreateWidget<USlot>(this, SlotWidgetClass);
        if (NewSlotWidget)
        {
            //NewSlotWidget->SetSlotType(ItemTypeTag);
            NewSlotWidget->SlotIndex = i;
            NewSlotWidget->OnSlotMouseEnter.AddDynamic(this, &UItemListPanel::OnSlotMouseEntered);
            SlotPool.Add(NewSlotWidget);
            UniformGridPanel->AddChildToUniformGrid(NewSlotWidget,(i / NumColumns),(i % NumColumns));
        }
    }

    //if(SlotsInfos.)
    //UpdateSlots(SlotsInfos,Size);
}

void UItemListPanel::ClearSlots()
{
    for (UUserWidget* SlotWidget : SlotPool)
    {
        if (USlot* PanelSlot = Cast<USlot>(SlotWidget))
        {
            PanelSlot->ClearSlot();  // 슬롯 데이터 초기화
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
            PanelSlot->UpdateSlot(ItemInfo);  
            if (ItemInfo.Icon!=nullptr && !ItemInfo.Name.EqualTo(FText::FromString("-"))) //아이템을 슬롯에 저장하고 있지 않은 경우
            {
                if (!PanelSlot->OnSlotClicked.IsBound())
                {
                    PanelSlot->OnSlotClicked.AddDynamic(this, &UItemListPanel::OnSlotClicked);
                }
            }
            else
            {
                if (PanelSlot->OnSlotClicked.IsBound())
                {
                    PanelSlot->OnSlotClicked.RemoveDynamic(this, &UItemListPanel::OnSlotClicked);
                }
            }
        }
    }
}

void UItemListPanel::OnSlotClicked(USlot* ClickedSlot)
{
    // SlotPool에서 ClickedSlot의 인덱스를 검색
    if (UItemListPaenlPresenter* ItemListPanelPresenter = Cast<UItemListPaenlPresenter>(GetPresenter()))
    {
        
        int32 SlotIndex = SlotPool.IndexOfByKey(ClickedSlot);

        if (SlotIndex != INDEX_NONE)  // 유효한 인덱스 확인
        {
            this->SetVisibility(ESlateVisibility::Collapsed);
            OnItemListSlotClickedDelegate.Broadcast(ItemListPanelPresenter->GetCurItemListType(), ClickedSlot->SlotIndex);
        }
    }
}

void UItemListPanel::OnSlotMouseEntered(USlot* EnteredSlot)
{
    // SlotPool에서 ClickedSlot의 인덱스를 검색
    int32 SlotIndex = SlotPool.IndexOfByKey(EnteredSlot);

    if (SlotIndex != INDEX_NONE)  // 유효한 인덱스 확인
    {
        if (UItemListPaenlPresenter* ItemListPanelPresenter = Cast<UItemListPaenlPresenter>(GetPresenter()))
        {
            FEquipmentSlotClickedMessage Msg(ItemListPanelPresenter->GetCurItemListType(), SlotIndex);
            
            ItemListPanelPresenter->OnItemListSlotEntered(Msg);
        }
    
        /*
        UEquipmentPresenter* EquipPresenter = Cast<UEquipmentPresenter>(GetPresenter());
        EquipPresenter->OnItemListSlotEntered(EnteredSlot->GetSlotType(), SlotIndex);*/       
    }
}

void UItemListPanel::UpdateSlots(const TArray<FSlotDisplayInfo>& SlotsInfos , uint8 Num)
{
    for (int i = 0; i < SlotsInfos.Num(); i++)
    {
        UpdateSlot(i, SlotsInfos[i]);
    }
}

void UItemListPanel::WidgetPresenterSet()
{

    //UEquipmentPresenter* EquipPresenter = Cast<UEquipmentPresenter>(GetPresenter());

    //ModelDataChangeEvent
   // EquipPresenter->OnEquipItemEvent.AddDynamic(this, &UEquipmentWidget::UpdateSlot);
}
