// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/Datas/EquipBaseItemDataAsset.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ActionFramework/Interface/Useable.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"
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

		RegisteredItemContainer.Add(ItemTag, TArray<TObjectPtr<UItemBaseDataAsset>>());
		RegisteredItemContainer[ItemTag].Init(nullptr,GetEquipmentItemCapaicty(ItemTag));

		ActiveRegisteredItemIndexMap.Add({ ItemTag,0 });
	}
	// 초기 아이템 추가
	AddStartingItem();
	
}



UItemBaseDataAsset* UInventoryComponent::GetCurrentEquipWeaponData()
{
	FGameplayTag WeaponTag = ARPGGameplayTags::ItemType_Equipment_Weapon;
	uint8* CurIndex = ActiveRegisteredItemIndexMap.Find(WeaponTag);

	if (RegisteredItemContainer[WeaponTag][*CurIndex] != nullptr)
	{
		return RegisteredItemContainer[WeaponTag][*CurIndex];
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

void UInventoryComponent::SpawnEquipItemAndEquip(const UEquipBaseItemDataAsset* EquipDataAsset)
{
	if (!EquipDataAsset || !EquipDataAsset->ActorToSpawnClass) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerPawn;

	AEquipItem* SpawnedItem = GetWorld()->SpawnActor<AEquipItem>(
		EquipDataAsset->ActorToSpawnClass,
		OwnerPawn->GetActorTransform(),
		SpawnParams
	);

	if (SpawnedItem)
	{
		SpawnedItem->Init(EquipDataAsset);
		SpawnedItem->EquipMesh(EquipDataAsset);
		EquippedItemActors.Add({ EquipDataAsset->ItemTypeTag, SpawnedItem });
	}
}

void UInventoryComponent::GrantAbilitiesFromEquipItemData(const UEquipBaseItemDataAsset* EquipDataAsset)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerPawn);
	UAbilitySystemComponent* ASC = ASI ? ASI->GetAbilitySystemComponent() : nullptr;
	if (!ASC || !EquipDataAsset) return;

	for (const auto& AbilityClass : EquipDataAsset->WeaponData.Abilties)
	{
		if (!AbilityClass.Ability) continue;

		UARPGAbility* AbilityCDO = AbilityClass.Ability->GetDefaultObject<UARPGAbility>();
		FGameplayAbilitySpec Spec(AbilityCDO, 1, INDEX_NONE,AbilityClass.SourceObject);
		Spec.DynamicAbilityTags.AddTag(AbilityCDO->StartupInputTag);

		FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
		
		FGrantedAbilityHandles& Handles = GrantedAbilityMap.FindOrAdd(const_cast<UEquipBaseItemDataAsset*>(EquipDataAsset));
		Handles.Handles.Add(Handle);
	}
}

void UInventoryComponent::RemoveAbilitiesFromEquipItemData(const UEquipBaseItemDataAsset* EquipDataAsset)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerPawn);
	UAbilitySystemComponent* ASC = ASI ? ASI->GetAbilitySystemComponent() : nullptr;

	
	const FGrantedAbilityHandles& HantedAbilityHanddles = GrantedAbilityMap.FindAndRemoveChecked(EquipDataAsset);

	for (const auto& Handle : HantedAbilityHanddles.Handles)
	{
		ASC->ClearAbility(Handle);
	}
}



uint8 UInventoryComponent::GetEquipmentItemCapaicty(FGameplayTag ItemType)
{
	if (RegisteredItemContainerCapacity.Contains(ItemType))
	{
		return RegisteredItemContainerCapacity[ItemType];
	}

	return INDEX_NONE;
}

AActor* UInventoryComponent::GetSpawnedEquippedActor(FGameplayTag ItemTypeTag) const
{
	if (const TObjectPtr<AEquipItem>* Found = EquippedItemActors.Find(ItemTypeTag))
	{
		return *Found;
	}
		
	return nullptr;
}

void UInventoryComponent::ChangeNextWeapon(float ChangedIndex)
{
	const FGameplayTag WeaponTag = ARPGGameplayTags::ItemType_Equipment_Weapon;

	uint8* CapacityPtr = RegisteredItemContainerCapacity.Find(WeaponTag);
	uint8* CurIndexPtr = ActiveRegisteredItemIndexMap.Find(WeaponTag);
	const TArray<TObjectPtr<UItemBaseDataAsset>>* ItemsPtr = RegisteredItemContainer.Find(WeaponTag);

	if (!CapacityPtr || !CurIndexPtr || !ItemsPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : ChangeNextWeapon - Missing data "), *FString(__FILE__));
	}

	const uint8 Capcity = *CapacityPtr;
	uint8& CurIndex = *CurIndexPtr;

	if (AActor* EquippedActor = GetSpawnedEquippedActor(WeaponTag))
	{
		RemoveAbilitiesFromEquipItemData(Cast<UEquipBaseItemDataAsset>((*ItemsPtr)[CurIndex]));
		EquippedActor->Destroy(); 
		EquippedItemActors.Remove(WeaponTag);
	}

	CurIndex += ChangedIndex;
	if (CurIndex >= Capcity)
	{
		CurIndex = 0;
	}
	else if (CurIndex < 0)
	{
		CurIndex = Capcity;
	}

	UE_LOG(LogTemp, Warning, TEXT("Change Weapon Index %d"), CurIndex);

	// 새 아이템 가져오기
	if (UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>((*ItemsPtr)[CurIndex]))
	{
		SpawnEquipItemAndEquip(EquipData); // 액터 스폰 및 메시 장착 등
		GrantAbilitiesFromEquipItemData(EquipData);
	}
}

void UInventoryComponent::RegisterItemFromInventoryToSlot(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex)
{
	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = InventoryItemContainer.Find(ItemTypeTag))
	{
		if (Items->IsValidIndex(RequsetItemIndex))
		{
			UItemBaseDataAsset* SelectedItem = (*Items)[RequsetItemIndex];
			UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(SelectedItem);

			//장착 및 UI 갱신
			if (EquipData)
			{
				//현재 사용중인 인덱스와 장착하는 인덱스가 같은 경우에는 스폰 및 어빌리티 부여를 합니다.
				uint8* Index = ActiveRegisteredItemIndexMap.Find(ItemTypeTag);
				if (UpdateSlotIndex == *Index)
				{
						RegisteredItemContainer[ItemTypeTag][UpdateSlotIndex] = SelectedItem;
						OnEquipmentChange.Broadcast(ItemTypeTag, UpdateSlotIndex, SelectedItem);
						SpawnEquipItemAndEquip(EquipData);
						GrantAbilitiesFromEquipItemData(EquipData);
				}
				else
				{
					RegisteredItemContainer[ItemTypeTag][UpdateSlotIndex] = SelectedItem;
					OnEquipmentChange.Broadcast(ItemTypeTag, UpdateSlotIndex, SelectedItem);
				}
			}
		}
	}
}
void UInventoryComponent::UnRegisterItemFromSlot(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex)
{
	const TArray<TObjectPtr<UItemBaseDataAsset>>* Items = RegisteredItemContainer.Find(ItemTypeTag);
	if(Items->IsValidIndex(RequsetItemIndex))
	{
		UItemBaseDataAsset* SelectedItem = (*Items)[RequsetItemIndex];
		UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(SelectedItem);
		uint8* Index = ActiveRegisteredItemIndexMap.Find(ItemTypeTag);

		if (UpdateSlotIndex == *Index)
		{
			if (EquipData)
			{

				RegisteredItemContainer[ItemTypeTag][UpdateSlotIndex] = nullptr;
				OnEquipmentChange.Broadcast(ItemTypeTag,UpdateSlotIndex, nullptr);
			}
		}
		else
		{
			RegisteredItemContainer[ItemTypeTag][UpdateSlotIndex] = nullptr;
			OnEquipmentChange.Broadcast(ItemTypeTag, UpdateSlotIndex, nullptr);
		}

		UEquipBaseItemDataAsset* EquipDataAsset = Cast<UEquipBaseItemDataAsset>(SelectedItem);

		if (EquipDataAsset)
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerPawn);
			if (ASI)
			{
				UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
				if (ASC)
				{
					FGrantedAbilityHandles* Handles = GrantedAbilityMap.Find(EquipDataAsset);
					if (Handles)
					{
						for (const FGameplayAbilitySpecHandle& Handle : Handles->Handles)
						{
							ASC->ClearAbility(Handle);
						}
						GrantedAbilityMap.Remove(EquipDataAsset);
					}
				}
			}
		}
	}
}

bool UInventoryComponent::IsEquippedItem(FGameplayTag ItemTypeTag, uint8 Index)
{
	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = InventoryItemContainer.Find(ItemTypeTag))
	{
		if (Items->IsValidIndex(Index))
		{
			if (RegisteredItemContainer[ItemTypeTag].Contains((*Items)[Index]))
				return true;
		}
	}
	return false;
	
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
	if (const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundPtr = RegisteredItemContainer.Find(ItemType))
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

const UItemBaseDataAsset* UInventoryComponent::GetRegistedItem(FGameplayTag ItemType, uint8 Index)
{
	// 1) ItemType에 해당하는 배열을 찾는다
	const TArray<TObjectPtr<UItemBaseDataAsset>>* FoundArray = RegisteredItemContainer.Find(ItemType);
	if (!FoundArray)
	{
		// 로그를 남기고 nullptr 반환
		UE_LOG(LogTemp, Warning, TEXT("GetRegistedItem: No container found for ItemType [%s]"), *ItemType.ToString());
		return nullptr;
	}

	// 2) 인덱스 범위가 유효한지 확인
	if (!FoundArray->IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetRegistedItem: Index [%d] is out of range for ItemType [%s]. Size=[%d]"),
			Index, *ItemType.ToString(), FoundArray->Num());
		return nullptr;
	}

	// 3) 실제 아이템 확인
	const UItemBaseDataAsset* FoundItem = (*FoundArray)[Index];
	if (!FoundItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetRegistedItem: Found a null pointer at Index [%d] in container for ItemType [%s]"),
			Index, *ItemType.ToString());
		return nullptr;
	}

	// 4) 정상적인 아이템이 있으므로 그대로 반환
	return FoundItem;
}

bool UInventoryComponent::IsItemEquipped(UItemBaseDataAsset* Item)
{
	TArray<TObjectPtr<UItemBaseDataAsset>>* Items = RegisteredItemContainer.Find(Item->ItemTypeTag);

	if (Items->Contains(Item))
	{
		return  true;
	}
	return false;
}




