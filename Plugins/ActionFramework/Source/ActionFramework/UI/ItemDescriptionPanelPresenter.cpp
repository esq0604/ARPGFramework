// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ItemDescriptionPanelPresenter.h"
#include "ActionFramework/UI/EquipmentPresenterMediator.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/UI/ItemDescriptionPanel.h"
#include "ActionFramework/Inventory/InventoryComponent.h"

void UItemDescriptionPanelPresenter::BroadcastInitialValues()
{

}

void UItemDescriptionPanelPresenter::BindCallBacksToDependencies()
{
	if (GetPresenterMediator())
	{
		GetPresenterMediator()->OnEquipmentSlotEnteredDelegate.AddDynamic(this, &UItemDescriptionPanelPresenter::UpdateEquipmentItemInfoPanel);
		GetPresenterMediator()->OnItemListSlotEnteredDelegate.AddDynamic(this, &UItemDescriptionPanelPresenter::UpdateInventoryItemInfoPanel);
	}

}

void UItemDescriptionPanelPresenter::UpdateInventoryItemInfoPanel(const FEquipmentSlotClickedMessage& Msg)
{

	if (UItemDescriptionPanel* ItemDescriptionPanel = Cast<UItemDescriptionPanel>(GetView()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryItemInfoPanel Slot Type %s,  Slot Index %d "), *Msg.ItemTypeTag.ToString(), Msg.SlotIndex);
		if (UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent()))
		{
			const UItemBaseDataAsset* Item= InventoryComp->GetInventoryItem(Msg.ItemTypeTag, Msg.SlotIndex);
			FDescriptionInfo Info;

			if (Item)
			{
				//FDescriptionInfo ItemDesInfo;

				//슬롯 업데이트에 필요한 정보들을 가져옵니다.
				Info.ItemDes = Item->TextData.Description;
				Info.ItemEffect = Item->TextData.UsingText;
				Info.ItemImage = Item->AssetData.Icon;
				Info.ItemName = Item->TextData.Name;
	
				ItemDescriptionPanel->UpdateDescription(Info);
			}
			else
			{
				Info.ItemDes = FText::FromString("-");
				Info.ItemEffect = FText::FromString("-");
				Info.ItemImage = nullptr;
				Info.ItemName = FText::FromString("-");
				ItemDescriptionPanel->UpdateDescription(Info);

			}
		}

	}
}

void UItemDescriptionPanelPresenter::UpdateEquipmentItemInfoPanel(const FEquipmentSlotClickedMessage& Msg)
{
	if (UItemDescriptionPanel* ItemDescriptionPanel = Cast<UItemDescriptionPanel>(GetView()))
	{
		if (UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent()))
		{
			const UItemBaseDataAsset* Item = InventoryComp->GetEquipmentItem(Msg.ItemTypeTag, Msg.SlotIndex);
			FDescriptionInfo Info;

			if (Item)
			{
				//FDescriptionInfo ItemDesInfo;

				//슬롯 업데이트에 필요한 정보들을 가져옵니다.
				Info.ItemDes = Item->TextData.Description;
				Info.ItemEffect = Item->TextData.UsingText;
				Info.ItemImage = Item->AssetData.Icon;
				Info.ItemName = Item->TextData.Name;

				ItemDescriptionPanel->UpdateDescription(Info);
			}
			else
			{
				Info.ItemDes = FText::FromString("-");
				Info.ItemEffect = FText::FromString("-");
				Info.ItemImage = nullptr;
				Info.ItemName = FText::FromString("-");
				ItemDescriptionPanel->UpdateDescription(Info);

			}
		}

	}
}
