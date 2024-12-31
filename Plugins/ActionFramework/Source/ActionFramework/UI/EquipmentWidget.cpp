// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/UI/ItemListPanel.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"

void UEquipmentWidget::NativeConstruct()
{
    if (ItemListPanel)
    {
        ItemListPanel->InitializeSlot(SlotCapacity);
        ItemListPanel->OnItemListSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleItemListSlotClicked);
//        ItemListPanel->OnItemListSlotEntered.AddDynamic(this,)
    }
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UEquipmentWidget::HandleCloseButtonClicked);
	}

    if (WeaponPanel)
    {
        for (int32 i = 0; i < NumWeaponSlot; i++)  // 무기 슬롯 3개 예시
        {
            USlot* NewSlot = CreateWidget<USlot>(this, SlotWidgetClass);
            NewSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
            WeaponPanel->AddChildToUniformGrid(NewSlot, i / NumWeaponSlot, i % NumWeaponSlot);  // 행, 열 추가
            SlotTagMap.Add(NewSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Weapon")));
            

        }
    }

    if (ToolPanel)
    {
        for (int32 i = 0; i < NumToolSlot; i++)  
        {
            USlot* NewSlot = CreateWidget<USlot>(this, SlotWidgetClass);
            NewSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
            ToolPanel->AddChildToUniformGrid(NewSlot, i / 3, i % 3);  // 행, 열 추가
            SlotTagMap.Add(NewSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Tool")));

        }
    }

    ArmorSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    ShoesSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    GlovesSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    HelmetSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);

    // 개별 슬롯에 Tag 매핑
    SlotTagMap.Add(ArmorSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Armor")));
    SlotTagMap.Add(GlovesSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Gloves")));
    SlotTagMap.Add(HelmetSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Helmet")));
    SlotTagMap.Add(ShoesSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Shoes")));

    // 태그와 패널 매핑
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Weapon")), WeaponPanel);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Tool")), ToolPanel);

    // 개별 슬롯 태그는 nullptr로 처리 (개별 슬롯은 Panel이 없기 때문)
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Armor")), nullptr);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Helmet")), nullptr);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Shoes")), nullptr);
}

void UEquipmentWidget::HandleCloseButtonClicked()
{
    // ItemListPanel 숨기기
    if (ItemListPanel)
    {
        ItemListPanel->SetVisibility(ESlateVisibility::Collapsed);
    }

    // WrapperPanel 보이기
    if (SlotWrapperPanel)
    {
        SlotWrapperPanel->SetVisibility(ESlateVisibility::Visible);
    }

	OnCloseButtonClicked.Broadcast();
}

void UEquipmentWidget::HandleEquipmentSlotClicked(USlot* ClickedSlot)
{
    //슬롯이 보유가능한 아이템 타입에 해당하는 ItemListPanel로 업데이트하도록 InventoryComponent에 이벤트를 보냅니다.
     // 클릭된 Slot이 어떤 Panel에 속해 있는지 확인
    if (SlotTagMap.Contains(ClickedSlot))
    {
        EquipRequestSlot = ClickedSlot;
        FGameplayTag SlotTag = *SlotTagMap.Find(ClickedSlot);

        // InventoryComponent를 통해 아이템 목록 요청
        OnRequestItemList.Broadcast(SlotTag,this);

        // WrapperPanel 숨기기
        if (SlotWrapperPanel)
        {
            SlotWrapperPanel->SetVisibility(ESlateVisibility::Collapsed);
        }

        // ItemListPanel 보이기
        if (ItemListPanel)
        {
            ItemListPanel->SetVisibility(ESlateVisibility::Visible);
        }
    }
 
}

void UEquipmentWidget::UpdateItemListPanel(const TArray<FSlotDisplayInfo>& Infos)
{
    ItemListPanel->ClearSlots();

    for (int i = 0; i < Infos.Num(); i++)
    {
        ItemListPanel->UpdateSlot(i, Infos[i]);
    }
}

void UEquipmentWidget::UpdateEquipRequestSlot(const FSlotDisplayInfo& Info)
{
    EquipRequestSlot->UpdateItemInfo(Info);
    ItemListPanel->SetVisibility(ESlateVisibility::Collapsed);
    SlotWrapperPanel->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentWidget::HandleItemListSlotClicked(int32 ClickedSlotIndex)
{
    //ClickedSlotIndex와 CurOpenItemListPanel를 넘겨주는 델리게이트를 Broadcast하여 InventoryComponent에 아이템을 장착하고 업데이트하도록 요청해야합니다.
    int32 RequsetEquipItemSlotIndex = GetSlotIndex(CurOpenItemListPanel);
    
    OnRequestEquipItem.Broadcast(CurOpenItemListPanel, ClickedSlotIndex, RequsetEquipItemSlotIndex);
    UE_LOG(LogTemp, Log, TEXT("Requesting Equip Item with Tag: %s, Slot Index: %d"),
        *CurOpenItemListPanel.ToString(), ClickedSlotIndex);

    UE_LOG(LogTemp, Warning, TEXT("Requset Equip Item Slot Index : %d"), RequsetEquipItemSlotIndex);
}

void UEquipmentWidget::HandleItemListSlotEntered(int32 EnteredSlotIndex)
{
    // 패널이 있는 경우
    int32 RequsetEquipItemNameSlotIndex = GetSlotIndex(CurOpenItemListPanel);

    //OnRequestItemName.Broadcast(CurOpenItemListPanel, EnteredSlotIndex, RequsetEquipItemNameSlotIndex);
}

int32 UEquipmentWidget::GetSlotIndex(FGameplayTag ItemTypeTag)
{

      // 태그에 매핑된 패널 검색
    UUniformGridPanel** PanelPtr = TagPanelMap.Find(ItemTypeTag);

    // 패널이 있는 경우
    if (PanelPtr && *PanelPtr)
    {
        return (*PanelPtr)->GetChildIndex(EquipRequestSlot);
    }

    // 개별 슬롯의 경우
    if (SlotTagMap.Contains(EquipRequestSlot) && SlotTagMap[EquipRequestSlot] == ItemTypeTag)
    {
        return 0;
    }

    // 매칭되지 않을 경우
    return INDEX_NONE;
}

void UEquipmentWidget::SetCurOpenItemListPanel(FGameplayTag ItemTypeTag)
{
    CurOpenItemListPanel = ItemTypeTag;
}


