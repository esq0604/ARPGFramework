// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ActionFramework/Interface/Useable.h"
#include "ActionFramework/Items/WeaponItem.h"
//#include "uLang/Common/Misc/Optional.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

}

//hud -> ui 생성 -> ui에서 

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FGameplayTag> ItemTags = AllItemTypeTags.GetGameplayTagArray();
	for (const FGameplayTag& ItemTag : ItemTags)
	{
		InventoryItemContainer.Add(ItemTag, TArray<TObjectPtr<UItemBaseDataAsset>>());
		InventoryItemContainer[ItemTag].Init(nullptr, ItemContainerSize);

		EquipmentItemContainer.Add(ItemTag, TArray<TObjectPtr<UItemBaseDataAsset>>());
		EquipmentItemContainer[ItemTag].Init(nullptr,GetEquipmentItemCapaicty(ItemTag));

		CurUsingTagEquipmentContainerIndexMap.Add({ ItemTag,0 });
	}
	// 초기 아이템 추가
	AddStartingItem();
	
}



UItemBaseDataAsset* UInventoryComponent::GetCurrentEquipWeaponData()
{
	FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon");
	uint8* CurIndex = CurUsingTagEquipmentContainerIndexMap.Find(WeaponTag);

	if (EquipmentItemContainer[WeaponTag][*CurIndex] != nullptr)
	{
		return EquipmentItemContainer[WeaponTag][*CurIndex];
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
			AddItemToItemContainer(DuplicatedItemDataAsset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DuplicatedItemDataAsset null"));
		}
	}
}

void UInventoryComponent::AddItemToItemContainer(const UItemBaseDataAsset* AddedItem)
{
		if (!AddedItem)
			return;

		FGameplayTag ItemTypeTag = AddedItem->ItemTypeTag;

		if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = InventoryItemContainer.Find(ItemTypeTag))
		{
			int32 EmptySlotIndex = Items->IndexOfByKey(nullptr);
			// 1. 빈 슬롯(Null) 검색 및 아이템 추가
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

uint8 UInventoryComponent::GetEquipmentItemCapaicty(FGameplayTag ItemType)
{
	if (EquipmentItemContainerCapacity.Contains(ItemType))
	{
		return EquipmentItemContainerCapacity[ItemType];
	}

	return INDEX_NONE;
}

void UInventoryComponent::ChangeNextWeapon(float ChangedIndex)
{
	FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon");
	uint8* WeaponCapacity = EquipmentItemContainerCapacity.Find(FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon"));
	uint8* CurIndex = CurUsingTagEquipmentContainerIndexMap.Find(WeaponTag);

	const TArray<TObjectPtr<UItemBaseDataAsset>>* WeaponEquipContainer = EquipmentItemContainer.Find(WeaponTag);
	if ((*WeaponEquipContainer)[*CurIndex] != nullptr)
	{
		IEquipable* EquipableItem = Cast<IEquipable>((*WeaponEquipContainer)[*CurIndex]);
		if (EquipableItem)
		{
			EquipableItem->UnEquip();
		}
	}

	(*CurIndex) += ChangedIndex;
	if (*CurIndex >= *WeaponCapacity)
	{
		*CurIndex = 0;
	}
	else if (*CurIndex < 0)
	{
		*CurIndex = *WeaponCapacity;
	}

	UE_LOG(LogTemp, Warning, TEXT("Change Weapon Index %d"), *CurIndex);

	if ((*WeaponEquipContainer)[*CurIndex] != nullptr)
	{
		IEquipable* EquipableItem = Cast<IEquipable>((*WeaponEquipContainer)[*CurIndex]);
		if (EquipableItem)
		{
			EquipableItem->Equip();
		}
	}	
}

void UInventoryComponent::EquipItemFromInventoryItemContainer(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex)
{

	UE_LOG(LogTemp, Warning, TEXT("EquipItemFromInventoryItemContainer :: RequsetItemIndex %d , UpdateIndex %d"), RequsetItemIndex, UpdateSlotIndex);
	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = InventoryItemContainer.Find(ItemTypeTag))
	{
		if (Items->IsValidIndex(RequsetItemIndex))
		{
			UItemBaseDataAsset* SelectedItem = (*Items)[RequsetItemIndex];
			IEquipable* EquipableItem = Cast<IEquipable>((*Items)[RequsetItemIndex]);
			
			if (EquipableItem)
			{
				uint8* Index = CurUsingTagEquipmentContainerIndexMap.Find(ItemTypeTag);
				if(UpdateSlotIndex == *Index)
				{
					if (EquipableItem->Equip())
					{
						EquipmentItemContainer[ItemTypeTag][UpdateSlotIndex] = SelectedItem;
						OnEquipmentChange.Broadcast(ItemTypeTag,UpdateSlotIndex, SelectedItem);
					}
				}
				else
				{

					EquipmentItemContainer[ItemTypeTag][UpdateSlotIndex] = SelectedItem;
					OnEquipmentChange.Broadcast(ItemTypeTag,UpdateSlotIndex, SelectedItem);
				}
			}
		}
	}
}

void UInventoryComponent::UnEquipItem(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex)
{
	const TArray<TObjectPtr<UItemBaseDataAsset>>* Items = EquipmentItemContainer.Find(ItemTypeTag);
	if(Items->IsValidIndex(RequsetItemIndex))
	{
		UItemBaseDataAsset* SelectedItem = (*Items)[RequsetItemIndex];
		IEquipable* EquipableItem = Cast<IEquipable>((*Items)[RequsetItemIndex]);
		uint8* Index = CurUsingTagEquipmentContainerIndexMap.Find(ItemTypeTag);

		if (UpdateSlotIndex == *Index)
		{
			if (EquipableItem->UnEquip())
			{
				EquipmentItemContainer[ItemTypeTag][UpdateSlotIndex] = nullptr;
				OnEquipmentChange.Broadcast(ItemTypeTag,UpdateSlotIndex, nullptr);
			}
		}
		else
		{
			EquipmentItemContainer[ItemTypeTag][UpdateSlotIndex] = nullptr;
			OnEquipmentChange.Broadcast(ItemTypeTag, UpdateSlotIndex, nullptr);
		}
	}
}

bool UInventoryComponent::IsEquippedItem(FGameplayTag ItemTypeTag, uint8 Index)
{
	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = EquipmentItemContainer.Find(ItemTypeTag))
	{
		if (Items->IsValidIndex(Index))
		{
			if (UItemBaseDataAsset* SelectedItem = (*Items)[Index])
			{
				return true;
			}	
		}
	}
	return false;
	
}

void UInventoryComponent::EquipUnEquipItemToEquipment(FGameplayTag ItemType, int32 ContainerIndex , int32 EquipIndex)
{

	//if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemType))
	//{
	//	// 2. 유효한 ContainerIndex 확인
	//	if (Items->IsValidIndex(ContainerIndex))
	//	{
	//		UItemBaseDataAsset* SelectedItem = (*Items)[ContainerIndex];
	//		if (SelectedItem)
	//		{
	//			TArray<TObjectPtr<UItemBaseDataAsset>>* EquipItems = EquipmentItemContainer.Find(ItemType);
	//			// 같은 아이템 클릭시 기존 장비 해제
	//			if (EquipItems && EquipItems->IsValidIndex(EquipIndex))
	//			{
	//				UItemBaseDataAsset* CurrentlyEquippedItem = (*EquipItems)[EquipIndex];
	//				if (CurrentlyEquippedItem && CurrentlyEquippedItem->Implements<UEquipable>()) 
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("UnEquip"));
	//					IEquipable* EquipableItem = Cast<IEquipable>(CurrentlyEquippedItem);
	//					EquipableItem->UnEquip();
	//					(*EquipItems)[EquipIndex] = nullptr;

	//					if (EquipmentWidget)
	//					{
	//						FSlotDisplayInfo SlotInfo;
	//						SlotInfo.Icon = nullptr; // 또는 빈 아이콘 리소스 설정
	//						SlotInfo.Name = FText::FromString(TEXT("")); // 빈 텍스트
	//						SlotInfo.Count = 0; // 카운트는 0
	//						//EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
	//						return;
	//					}
	//				}
	//			}

	//			// 다른 아이템 클릭시 새 장비 장착
	//			if (EquipItems) 
	//			{
	//				(*EquipItems)[EquipIndex] = SelectedItem;
	//				if (SelectedItem->Implements<UEquipable>())
	//				{
	//					IEquipable* EquipableItem = Cast<IEquipable>(SelectedItem);
	//					EquipableItem->Equip();
	//				}
	//				// 4. EquipmentWidget 업데이트
	//				if (EquipmentWidget)
	//				{
	//					FSlotDisplayInfo SlotInfo;
	//					SlotInfo.Icon = SelectedItem->AssetData.Icon;
	//					SlotInfo.Name = SelectedItem->TextData.Name;
	//					SlotInfo.Count = SelectedItem->NumericData.Quantity;

	//					//EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
	//				}
	//			}
	//		}
	//	}
	//}
}

const TArray<TObjectPtr<UItemBaseDataAsset>>* UInventoryComponent::GetInventoryItems(FGameplayTag ItemType)
{

	if (const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundPtr = InventoryItemContainer.Find(ItemType))
	{
		return FoundPtr;  
	}
	
	return nullptr;
}

const TArray<TObjectPtr<UItemBaseDataAsset>>* UInventoryComponent::GetEquipmentItems(FGameplayTag ItemType)
{
	if (const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundPtr = EquipmentItemContainer.Find(ItemType))
	{
		return FoundPtr;
	}
	return nullptr;
}

const UItemBaseDataAsset* UInventoryComponent::GetInventoryItem(FGameplayTag ItemType, uint8 Index)
{
	// 1) ItemType에 해당하는 배열을 찾는다
	const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundArray = InventoryItemContainer.Find(ItemType);
	if (!FoundArray)
	{
		// 로그를 남기고 nullptr 반환
		UE_LOG(LogTemp, Warning, TEXT("GetInventory: No container found for ItemType [%s]"), *ItemType.ToString());
		return nullptr;
	}

	// 2) 인덱스 범위가 유효한지 확인
	if (!FoundArray->IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInventory: Index [%d] is out of range for ItemType [%s]. Size=[%d]"),
			Index, *ItemType.ToString(), FoundArray->Num());
		return nullptr;
	}

	// 3) 실제 아이템 확인
	const UItemBaseDataAsset* FoundItem = (*FoundArray)[Index];
	if (!FoundItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInventory: Found a null pointer at Index [%d] in container for ItemType [%s]"),
			Index, *ItemType.ToString());
		return nullptr;
	}

	// 4) 정상적인 아이템이 있으므로 그대로 반환
	return FoundItem;
}

const UItemBaseDataAsset* UInventoryComponent::GetEquipmentItem(FGameplayTag ItemType, uint8 Index)
{
	// 1) ItemType에 해당하는 배열을 찾는다
	const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundArray = EquipmentItemContainer.Find(ItemType);
	if (!FoundArray)
	{
		// 로그를 남기고 nullptr 반환
		UE_LOG(LogTemp, Warning, TEXT("GetEquipmentItem: No container found for ItemType [%s]"), *ItemType.ToString());
		return nullptr;
	}

	// 2) 인덱스 범위가 유효한지 확인
	if (!FoundArray->IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetEquipmentItem: Index [%d] is out of range for ItemType [%s]. Size=[%d]"),
			Index, *ItemType.ToString(), FoundArray->Num());
		return nullptr;
	}

	// 3) 실제 아이템 확인
	const UItemBaseDataAsset* FoundItem = (*FoundArray)[Index];
	if (!FoundItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetEquipmentItem: Found a null pointer at Index [%d] in container for ItemType [%s]"),
			Index, *ItemType.ToString());
		return nullptr;
	}

	// 4) 정상적인 아이템이 있으므로 그대로 반환
	return FoundItem;
}

bool UInventoryComponent::IsItemEquipped(UItemBaseDataAsset* Item)
{
	TArray<TObjectPtr<UItemBaseDataAsset>>* Items = EquipmentItemContainer.Find(Item->ItemTypeTag);

	if (Items->Contains(Item))
	{
		return  true;
	}
	return false;
}




