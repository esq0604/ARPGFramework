// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/InventoryWidget.h"
#include "ActionFramework/UI/ItemListPanel.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	// ���� ���� �Լ��� �� ��ư�� �̺�Ʈ ���ε�
	if (WeaponButton) WeaponButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnWeaponButtonClicked);
	if (ToolButton) ToolButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnToolButtonClicked);
	if (HelmetButton) HelmetButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnHelmetButtonClicked);
	if (ArmorButton) ArmorButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnArmorButtonClicked);
	if (GlovesButton) GlovesButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnGlovesButtonClicked);
	if (ShoesButton) ShoesButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnShoesButtonClicked);

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::HandleCloseButtonClicked);
	}

	/*if (ItemListPanel)
	{
		ItemListPanel->InitializeSlot(SlotCapacity);
	}*/
}

//void UInventoryWidget::UpdateItemListPanel(const TArray<FSlotDisplayInfo>& Infos)
//{
//	ItemListPanel->ClearSlots();
//
//	for (int i = 0; i < Infos.Num(); i++)
//	{
//		ItemListPanel->UpdateSlot(i, Infos[i]);
//	}
//}

void UInventoryWidget::HandleButtonClicked(FGameplayTag ItemTypeTag)
{
	//ItemTypeTag�� �Ű������� �Ѱ��ִ� ��������Ʈ�� boradcast�Ͽ� InventoryComponent���� Ÿ�Կ� �ش��ϴ� ������ ������� Update�ϵ����մϴ�.
	OnItemTypeSelected.Broadcast(ItemTypeTag,this);
}

void UInventoryWidget::OnWeaponButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")));
}

void UInventoryWidget::OnToolButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")));
}

void UInventoryWidget::OnHelmetButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")));
}

void UInventoryWidget::OnArmorButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")));
}

void UInventoryWidget::OnGlovesButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")));
}

void UInventoryWidget::OnShoesButtonClicked()
{
	HandleButtonClicked(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")));
}

void UInventoryWidget::HandleCloseButtonClicked()
{
	OnCloseButtonClicked.Broadcast();
}

void UInventoryWidget::SetCurOpenItemListPanel(FGameplayTag ItemTypeTag)
{
	CurOpenItemListPanel = ItemTypeTag;
}
