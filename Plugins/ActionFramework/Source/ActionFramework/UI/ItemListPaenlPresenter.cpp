// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ItemListPaenlPresenter.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/UI/EquipmentPresenter.h"
#include "EquipmentPresenterMediator.h"
#include  "ActionFramework/UI/ItemListPanel.h"



void UItemListPaenlPresenter::BroadcastInitialValues()
{

}

void UItemListPaenlPresenter::BindCallBacksToDependencies()
{
	if (UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent()))
	{
		uint8 ContaierSize = InventoryComp->GetInventoryItemContainerSize();

		UItemListPanel* ItemListPanel = Cast<UItemListPanel>(GetView());

		if (ItemListPanel)
		{
			ItemListPanel->InitializeSlot(ContaierSize);
			ItemListPanel->OnItemListSlotClickedDelegate.AddDynamic(this, &UItemListPaenlPresenter::EquipUnEquipItem);
		}
	}
	
	if (GetPresenterMediator())
	{
		GetPresenterMediator()->OnEquipmentSlotClickedDelegate.AddDynamic(this, &UItemListPaenlPresenter::OnItemListPanelUpdate);
	}
}

void UItemListPaenlPresenter::OnItemListPanelUpdate(const FEquipmentSlotClickedMessage& Msg)
{
	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent());
	CurClickedCatogoryIndex = Msg.SlotIndex;
	const TArray<TObjectPtr<UItemBaseDataAsset>>* InventoryItems = InventoryComp->GetInventoryItems(Msg.ItemTypeTag);
	TArray<FSlotDisplayInfo> SlotInfos;

	//슬롯 업데이트에 필요한 정보들을 가져옵니다.
	if (!InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("No items for Tag: %s"), *Msg.ItemTypeTag.ToString());
		return;
	}

	for (int i = 0; i < (*InventoryItems).Num(); i++)
	{
		FSlotDisplayInfo Info;

		if ((*InventoryItems)[i] != nullptr)
		{

			Info.Icon = (*InventoryItems)[i]->AssetData.Icon;
			Info.Name = (*InventoryItems)[i]->TextData.Name;
			Info.Count = (*InventoryItems)[i]->NumericData.Quantity;

			if (InventoryComp->IsItemEquipped((*InventoryItems)[i]))
			{
				Info.bIsEquipped = true;
			}
			else
			{
				Info.bIsEquipped = false;

			}
		}
		SlotInfos.Add(Info);
	}

	if (SlotInfos.Num() != 0)
	{
		UItemListPanel* ItemListPanel = Cast<UItemListPanel>(GetView());
		if (ItemListPanel)
		{
			CurItemListType = Msg.ItemTypeTag;
			ItemListPanel->UpdateSlots(SlotInfos, SlotInfos.Num());
		}
	}
}

void UItemListPaenlPresenter::OnItemListSlotEntered(const FEquipmentSlotClickedMessage& Msg)
{
	if (GetPresenterMediator())
	{
		GetPresenterMediator()->OnItemListSlotEnteredDelegate.Broadcast(Msg);
	}
}

void UItemListPaenlPresenter::EquipUnEquipItem(FGameplayTag ItemType, uint8 InventoryListClickedSlotIndex)
{
	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent());
	if (!InventoryComp->IsEquippedItem(ItemType, InventoryListClickedSlotIndex))
	{
		InventoryComp->EquipItemFromInventoryItemContainer(ItemType, InventoryListClickedSlotIndex, CurClickedCatogoryIndex);
	}
	else
	{
		InventoryComp->UnEquipItem(ItemType, InventoryListClickedSlotIndex, CurClickedCatogoryIndex);
	}
}
