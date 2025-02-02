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

    // ���� ���� ���� �� Ǯ �ʱ�ȭ
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
            PanelSlot->ClearSlot();  // ���� ������ �ʱ�ȭ
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
            if (ItemInfo.Icon!=nullptr && !ItemInfo.Name.EqualTo(FText::FromString("-"))) //�������� ���Կ� �����ϰ� ���� ���� ���
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
    // SlotPool���� ClickedSlot�� �ε����� �˻�
    if (UItemListPaenlPresenter* ItemListPanelPresenter = Cast<UItemListPaenlPresenter>(GetPresenter()))
    {
        
        int32 SlotIndex = SlotPool.IndexOfByKey(ClickedSlot);

        if (SlotIndex != INDEX_NONE)  // ��ȿ�� �ε��� Ȯ��
        {
            this->SetVisibility(ESlateVisibility::Collapsed);
            OnItemListSlotClickedDelegate.Broadcast(ItemListPanelPresenter->GetCurItemListType(), ClickedSlot->SlotIndex);
        }
    }
}

void UItemListPanel::OnSlotMouseEntered(USlot* EnteredSlot)
{
    // SlotPool���� ClickedSlot�� �ε����� �˻�
    int32 SlotIndex = SlotPool.IndexOfByKey(EnteredSlot);

    if (SlotIndex != INDEX_NONE)  // ��ȿ�� �ε��� Ȯ��
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
