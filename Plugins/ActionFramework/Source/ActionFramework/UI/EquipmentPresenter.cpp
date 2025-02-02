// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EquipmentPresenter.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "EquipmentPresenterMediator.h"
#include "Components/Image.h"

//InventoryComp->GetInventoryItemConinerSize를 통해 ItemListPanel의 개수를 초기화해놯야합니다.
//OnEquipmentSlotInit랑 비슷하게 하면됨.

UEquipmentPresenter::UEquipmentPresenter()
{

}

//장비에서 3가지 무기를 장착하고 교체한다면 -> Widget에서 3가지 무기만 장착할수있도록 WeaponSlotPanel을 만들어야합니다.
void UEquipmentPresenter::BroadcastInitialValues()
{


}

void UEquipmentPresenter::BindCallBacksToDependencies()
{
	// View의 입력 바인딩
	if (UEquipmentWidget* EquipmentWidget = Cast<UEquipmentWidget>(GetView()))
	{
		EquipmentWidget->OnCategorySlotClickedDelegate.AddDynamic(this, &UEquipmentPresenter::OnCategorySlotClicked);
		EquipmentWidget->OnCategorySlotEnteredDelegate.AddDynamic(this, &UEquipmentPresenter::OnCategorySlotEntered);
	}

	// Model의 데이터 변경 바인딩
	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent());
	if (InventoryComp)
	{
		if (InventoryComp->OnEquipmentChange.IsAlreadyBound(this, &UEquipmentPresenter::OnItemEquiped))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnEquipped alread bound"));
		}
		else
		{
			InventoryComp->OnEquipmentChange.AddDynamic(this, &UEquipmentPresenter::OnItemEquiped);
		}
	}
}

void UEquipmentPresenter::OnCategorySlotClicked(FGameplayTag ItemType, uint8 CategorySlotClickedIndex)
{

	//이부분은 ItemListPanel을 업데이트하는 이벤트이므로 ItemListPresenter에서 처리해야할거같은데 입력은 EquipmentWidget에서 들어오고있다 이걸 어떻게 처리해야할까.

	//OnRequestInventoryItemList.Broadcast(InventoryComp->GetInventoryItemContainerSize(), SlotInfos, ItemType);

	if (GetPresenterMediator())
	{
		UE_LOG(LogTemp, Warning, TEXT("CategorySlotCliced Index %d "), CategorySlotClickedIndex);
		FEquipmentSlotClickedMessage Msg(ItemType, CategorySlotClickedIndex);
		SetCurClickedCategorySlotIndex(CategorySlotClickedIndex);
		GetPresenterMediator()->OnEquipmentSlotClickedDelegate.Broadcast(Msg);
	}
	

	if (UEquipmentWidget* EquipmentWidget = Cast<UEquipmentWidget>(GetView()))
	{
		EquipmentWidget->ShowItemListPanel();
	}
}

void UEquipmentPresenter::OnCategorySlotEntered(FGameplayTag ItemType, uint8 EnteredSlotIndex)
{
	OnCategorySlotMouseEntered.Broadcast(ItemType, EnteredSlotIndex);

	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOptionalActorComponent());
	
	FEquipmentSlotClickedMessage Msg(ItemType, EnteredSlotIndex);
	GetPresenterMediator()->OnEquipmentSlotEnteredDelegate.Broadcast(Msg);
}

void UEquipmentPresenter::SetCurClickedCategorySlotIndex(uint8 Idx)
{
	CurClickedCategorySlotIndex = Idx;
}

void UEquipmentPresenter::OnItemEquiped(FGameplayTag RequestTypeTag, uint8 Index, const UItemBaseDataAsset* Item)
{
	FSlotDisplayInfo Info;

	if(Item!=nullptr)
	{ 
		Info.Icon = Item->AssetData.Icon;
		Info.Name = Item->TextData.Name;
		Info.Count = Item->NumericData.Quantity;
		Info.bIsEquipped = true;
	}
	else
	{
		Info.Name = FText::FromString("-");
		Info.bIsEquipped = false;
	}

	OnEquipItemEvent.Broadcast(RequestTypeTag, Index, Info);
}

