// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EquipmentPresenter.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "EquipmentPresenterMediator.h"
#include "Components/Image.h"

//InventoryComp->GetInventoryItemConinerSize�� ���� ItemListPanel�� ������ �ʱ�ȭ�؇Q���մϴ�.
//OnEquipmentSlotInit�� ����ϰ� �ϸ��.

UEquipmentPresenter::UEquipmentPresenter()
{

}

//��񿡼� 3���� ���⸦ �����ϰ� ��ü�Ѵٸ� -> Widget���� 3���� ���⸸ �����Ҽ��ֵ��� WeaponSlotPanel�� �������մϴ�.
void UEquipmentPresenter::BroadcastInitialValues()
{


}

void UEquipmentPresenter::BindCallBacksToDependencies()
{
	// View�� �Է� ���ε�
	if (UEquipmentWidget* EquipmentWidget = Cast<UEquipmentWidget>(GetView()))
	{
		EquipmentWidget->OnCategorySlotClickedDelegate.AddDynamic(this, &UEquipmentPresenter::OnCategorySlotClicked);
		EquipmentWidget->OnCategorySlotEnteredDelegate.AddDynamic(this, &UEquipmentPresenter::OnCategorySlotEntered);
	}

	// Model�� ������ ���� ���ε�
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

	//�̺κ��� ItemListPanel�� ������Ʈ�ϴ� �̺�Ʈ�̹Ƿ� ItemListPresenter���� ó���ؾ��ҰŰ����� �Է��� EquipmentWidget���� �������ִ� �̰� ��� ó���ؾ��ұ�.

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

