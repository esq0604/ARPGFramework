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
        for (int32 i = 0; i < NumWeaponSlot; i++)  // ���� ���� 3�� ����
        {
            USlot* NewSlot = CreateWidget<USlot>(this, SlotWidgetClass);
            NewSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
            WeaponPanel->AddChildToUniformGrid(NewSlot, i / NumWeaponSlot, i % NumWeaponSlot);  // ��, �� �߰�
            SlotTagMap.Add(NewSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Weapon")));
            

        }
    }

    if (ToolPanel)
    {
        for (int32 i = 0; i < NumToolSlot; i++)  
        {
            USlot* NewSlot = CreateWidget<USlot>(this, SlotWidgetClass);
            NewSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
            ToolPanel->AddChildToUniformGrid(NewSlot, i / 3, i % 3);  // ��, �� �߰�
            SlotTagMap.Add(NewSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Tool")));

        }
    }

    ArmorSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    ShoesSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    GlovesSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);
    HelmetSlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::HandleEquipmentSlotClicked);

    // ���� ���Կ� Tag ����
    SlotTagMap.Add(ArmorSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Armor")));
    SlotTagMap.Add(GlovesSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Gloves")));
    SlotTagMap.Add(HelmetSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Helmet")));
    SlotTagMap.Add(ShoesSlot, FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Shoes")));

    // �±׿� �г� ����
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Weapon")), WeaponPanel);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Tool")), ToolPanel);

    // ���� ���� �±״� nullptr�� ó�� (���� ������ Panel�� ���� ����)
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Armor")), nullptr);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Helmet")), nullptr);
    TagPanelMap.Add(FGameplayTag::RequestGameplayTag(TEXT("ItemType.Equipment.Shoes")), nullptr);
}

void UEquipmentWidget::HandleCloseButtonClicked()
{
    // ItemListPanel �����
    if (ItemListPanel)
    {
        ItemListPanel->SetVisibility(ESlateVisibility::Collapsed);
    }

    // WrapperPanel ���̱�
    if (SlotWrapperPanel)
    {
        SlotWrapperPanel->SetVisibility(ESlateVisibility::Visible);
    }

	OnCloseButtonClicked.Broadcast();
}

void UEquipmentWidget::HandleEquipmentSlotClicked(USlot* ClickedSlot)
{
    //������ ���������� ������ Ÿ�Կ� �ش��ϴ� ItemListPanel�� ������Ʈ�ϵ��� InventoryComponent�� �̺�Ʈ�� �����ϴ�.
     // Ŭ���� Slot�� � Panel�� ���� �ִ��� Ȯ��
    if (SlotTagMap.Contains(ClickedSlot))
    {
        EquipRequestSlot = ClickedSlot;
        FGameplayTag SlotTag = *SlotTagMap.Find(ClickedSlot);

        // InventoryComponent�� ���� ������ ��� ��û
        OnRequestItemList.Broadcast(SlotTag,this);

        // WrapperPanel �����
        if (SlotWrapperPanel)
        {
            SlotWrapperPanel->SetVisibility(ESlateVisibility::Collapsed);
        }

        // ItemListPanel ���̱�
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
    //ClickedSlotIndex�� CurOpenItemListPanel�� �Ѱ��ִ� ��������Ʈ�� Broadcast�Ͽ� InventoryComponent�� �������� �����ϰ� ������Ʈ�ϵ��� ��û�ؾ��մϴ�.
    int32 RequsetEquipItemSlotIndex = GetSlotIndex(CurOpenItemListPanel);
    
    OnRequestEquipItem.Broadcast(CurOpenItemListPanel, ClickedSlotIndex, RequsetEquipItemSlotIndex);
    UE_LOG(LogTemp, Log, TEXT("Requesting Equip Item with Tag: %s, Slot Index: %d"),
        *CurOpenItemListPanel.ToString(), ClickedSlotIndex);

    UE_LOG(LogTemp, Warning, TEXT("Requset Equip Item Slot Index : %d"), RequsetEquipItemSlotIndex);
}

void UEquipmentWidget::HandleItemListSlotEntered(int32 EnteredSlotIndex)
{
    // �г��� �ִ� ���
    int32 RequsetEquipItemNameSlotIndex = GetSlotIndex(CurOpenItemListPanel);

    //OnRequestItemName.Broadcast(CurOpenItemListPanel, EnteredSlotIndex, RequsetEquipItemNameSlotIndex);
}

int32 UEquipmentWidget::GetSlotIndex(FGameplayTag ItemTypeTag)
{

      // �±׿� ���ε� �г� �˻�
    UUniformGridPanel** PanelPtr = TagPanelMap.Find(ItemTypeTag);

    // �г��� �ִ� ���
    if (PanelPtr && *PanelPtr)
    {
        return (*PanelPtr)->GetChildIndex(EquipRequestSlot);
    }

    // ���� ������ ���
    if (SlotTagMap.Contains(EquipRequestSlot) && SlotTagMap[EquipRequestSlot] == ItemTypeTag)
    {
        return 0;
    }

    // ��Ī���� ���� ���
    return INDEX_NONE;
}

void UEquipmentWidget::SetCurOpenItemListPanel(FGameplayTag ItemTypeTag)
{
    CurOpenItemListPanel = ItemTypeTag;
}


