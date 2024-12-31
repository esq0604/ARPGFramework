// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ActionFramework/Interface/Useable.h"
#include "ActionFramework/UI/InventoryWidget.h"
#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/UI/QuickSlotWidget.h"
#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/UI/SlotViewModel.h"
#include "MVVMGameSubSystem.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")), TArray<TObjectPtr<UItemBaseDataAsset>>());

	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")), TArray<TObjectPtr<UItemBaseDataAsset>>());

	// �� �±׿� ���� �ʱ�ȭ
	for (auto& Pair : ItemContainer)
	{
		Pair.Value.Init(nullptr, ItemContainerSize);
	}

	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")))->Init(nullptr, 3);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")))->Init(nullptr, 6);



}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AddStartingItem();

}

void UInventoryComponent::ToggleInventoryWidget()
{
	if (!InventoryWidget && InventoryWidgetClass)
	{

		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->OnCloseButtonClicked.AddDynamic(this, &UInventoryComponent::ToggleInventoryWidget);
		InventoryWidget->OnItemTypeSelected.AddDynamic(this, &UInventoryComponent::UpdateItemList);

		// ó�� ���� �ÿ��� AddToViewport ȣ��
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);  // �ʱ� ���´� ����
	}

	if (InventoryWidget)
	{
		// Visibility�� ������� UI ���
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetMouseCursorVisibility(false);

		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryComponent::ToggleEquipmentWidget()
{
	if (!EquipmentWidget && EquipmentWidgetClass)
	{
		EquipmentWidget = CreateWidget<UEquipmentWidget>(GetWorld(), EquipmentWidgetClass);
		////EquipmentWidget
		//EquipmentWidget->OnCloseButtonClicked.AddDynamic(this, &UInventoryComponent::ToggleEquipmentWidget);
		//EquipmentWidget->OnRequestItemList.AddDynamic(this, &UInventoryComponent::UpdateItemList);
		//EquipmentWidget->OnRequestEquipItem.AddDynamic(this, &UInventoryComponent::EquipUnEquipItemToEquipment);

		/*EquipmentViewModel = NewObject<UEquipmentViewModel>();
		if (EquipmentViewModel)
		{
			
		}*/
		
		
		// ó�� ���� �ÿ��� AddToViewport ȣ��
		EquipmentWidget->AddToViewport();
		EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);  // �ʱ� ���´� ����
	}

	if (EquipmentWidget)
	{
		// Visibility�� ������� UI ���
		if (EquipmentWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetMouseCursorVisibility(false);
		}
		else
		{
			EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryComponent::ToggleQuickSlotWidget()
{

}

UItemBaseDataAsset* UInventoryComponent::GetCurrentEquipWeaponData()
{
	if (CurEquipedItemData[FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon")][CurUsingWeaponIndex] != nullptr)
	{
		return CurEquipedItemData[FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon")][CurUsingWeaponIndex];
	}

	return nullptr;
}


void UInventoryComponent::AddStartingItem()
{
	for (const UItemBaseDataAsset* Item : StartingItems)
	{

		const UItemBaseDataAsset* DuplicatedItemDataAsset = DuplicateObject(Item, GetOwner());
		
		if (DuplicatedItemDataAsset != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DuplicatedItemDataAsset->GetOuter()->GetName());
			AddItemToItemContainer(DuplicatedItemDataAsset);
			AddItemToQuickSlotWidget(DuplicatedItemDataAsset);
		}
	}
}

void UInventoryComponent::AddItemToItemContainer(const UItemBaseDataAsset* AddedItem)
{
		if (!AddedItem)
			return;

		FGameplayTag ItemTypeTag = AddedItem->ItemTypeTag;

		if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemTypeTag))
		{
			int32 EmptySlotIndex = Items->IndexOfByKey(nullptr);
			// 1. �� ����(Null) �˻� �� ������ �߰�
			for (int32 i = 0; i < Items->Num(); ++i)
			{
				if ((*Items)[i] == nullptr)
				{
					(*Items)[i] = const_cast<UItemBaseDataAsset*>(AddedItem);
					break;
				}
			}
		}
}

void UInventoryComponent::AddItemToQuickSlotWidget(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::EquipItem(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::UnEquipItem(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::UseItem(const UItemBaseDataAsset* AddedItem)
{
}

//AActor* UInventoryComponent::SpawnedItemFromDataAsset(const UItemBaseDataAsset* InData)
//{
//	AItemBase* SpawnItem = nullptr;
//	if (InData == nullptr)
//		return nullptr;
//	if (InData->ActorToSpawnClass == nullptr)
//		return nullptr;
//
//	const FTransform SpawnTransform = GetOwner()->GetActorTransform();
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.Owner = GetOwner();
//	SpawnItem = GetWorld()->SpawnActor<AItemBase>(InData->ActorToSpawnClass, SpawnTransform, SpawnInfo);
//
//	return nullptr;
//}

void UInventoryComponent::EquipUnEquipItemToEquipment(FGameplayTag ItemType, int32 ContainerIndex , int32 EquipIndex)
{

	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemType))
	{
		// 2. ��ȿ�� ContainerIndex Ȯ��
		if (Items->IsValidIndex(ContainerIndex))
		{
			UItemBaseDataAsset* SelectedItem = (*Items)[ContainerIndex];
			if (SelectedItem)
			{
				TArray<TObjectPtr<UItemBaseDataAsset>>* EquipItems = CurEquipedItemData.Find(ItemType);
				// ���� ������ Ŭ���� ���� ��� ����
				if (EquipItems && EquipItems->IsValidIndex(EquipIndex))
				{
					UItemBaseDataAsset* CurrentlyEquippedItem = (*EquipItems)[EquipIndex];
					if (CurrentlyEquippedItem && CurrentlyEquippedItem->Implements<UEquipable>()) 
					{
						UE_LOG(LogTemp, Warning, TEXT("UnEquip"));
						IEquipable* EquipableItem = Cast<IEquipable>(CurrentlyEquippedItem);
						EquipableItem->UnEquip();
						(*EquipItems)[EquipIndex] = nullptr;

						if (EquipmentWidget)
						{
							FSlotDisplayInfo SlotInfo;
							SlotInfo.Icon = nullptr; // �Ǵ� �� ������ ���ҽ� ����
							SlotInfo.Name = FText::FromString(TEXT("")); // �� �ؽ�Ʈ
							SlotInfo.Count = 0; // ī��Ʈ�� 0
							EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
							return;
						}
					}
				}

				// �ٸ� ������ Ŭ���� �� ��� ����
				if (EquipItems) 
				{
					(*EquipItems)[EquipIndex] = SelectedItem;
					if (SelectedItem->Implements<UEquipable>())
					{
						IEquipable* EquipableItem = Cast<IEquipable>(SelectedItem);
						EquipableItem->Equip();
					}
					// 4. EquipmentWidget ������Ʈ
					if (EquipmentWidget)
					{
						FSlotDisplayInfo SlotInfo;
						SlotInfo.Icon = SelectedItem->AssetData.Icon;
						SlotInfo.Name = SelectedItem->TextData.Name;
						SlotInfo.Count = SelectedItem->NumericData.Quantity;

						EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
					}
				}
			}
		}
	}
}


void UInventoryComponent::UpdateItemList(FGameplayTag ItemTypeTag, UUserWidget* RequestingWidget)
{
	//ItemContainer���� ItemTypeTag�� �´� ������ �迭�� �����ͼ� InventoryWidget�� Update�ϵ����մϴ�.
		// ItemContainer���� ItemTypeTag�� �ش��ϴ� ������ �迭�� ������
	TArray<FSlotDisplayInfo> Infos;

	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemTypeTag))
	{
		for (const auto& Item : *Items)
		{
			if (Item)
			{
				Infos.Add(FSlotDisplayInfo(Item->AssetData.Icon, Item->NumericData.Quantity, Item->TextData.Name));
			}
		}
		// ȣ���� ������ InventoryWidget�� ���
		if (UInventoryWidget* InventoryWidgetInstance = Cast<UInventoryWidget>(RequestingWidget))
		{
			InventoryWidgetInstance->UpdateItemListPanel(Infos);
			//InventoryWidgetInstance->SetCurOpenItemListPanel(ItemTypeTag);
		}
		// ȣ���� ������ EquipmentWidget�� ���
		else if (UEquipmentWidget* EquipmentWidgetInstance = Cast<UEquipmentWidget>(RequestingWidget))
		{
			EquipmentWidgetInstance->UpdateItemListPanel(Infos);
			EquipmentWidgetInstance->SetCurOpenItemListPanel(ItemTypeTag);
		}
	}
}

void UInventoryComponent::UpdateEquipmentWiget(FGameplayTag ItemTypeTag)
{
}

void UInventoryComponent::SetMouseCursorVisibility(bool bIsVisible)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetShowMouseCursor(bIsVisible);
	}
}




