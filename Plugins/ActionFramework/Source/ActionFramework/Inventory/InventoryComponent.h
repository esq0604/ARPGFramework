// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpec.h"
#include "InventoryComponent.generated.h"

class UItemBaseDataAsset;
class UEquipBaseItemDataAsset;
class AEquipItem;
class UInventoryWidget;
class UEquipmentWidget;
class UQuickSlotWidget;
class UEquipmentPresenter;
//enum class EWeaponType : uint8;


USTRUCT()
struct FWeaponEquipItems
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AEquipItem*> Items;

	uint8 CurUsingIndex = 0;
};

USTRUCT()
struct FGrantedAbilityHandles
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> Handles;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentChangeDelegate,FGameplayTag, Tag, uint8 , Index, const UItemBaseDataAsset* , EquippedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ,BlueprintType ,Blueprintable)
class ACTIONFRAMEWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	//	virtual void InitializeComponent() override;

	const TArray<TObjectPtr<UItemBaseDataAsset>>* GetInventoryItems(FGameplayTag ItemType);
	const TArray<TObjectPtr<UItemBaseDataAsset>>* GetEquipmentItems(FGameplayTag ItemType);
	const UItemBaseDataAsset* GetInventoryItem(FGameplayTag ItemType, uint8 Index);
	const UItemBaseDataAsset* GetRegistedItem(FGameplayTag ItemType, uint8 Index);
	UItemBaseDataAsset* GetCurrentEquipWeaponData();
	
	FGameplayTagContainer GetAllItemType() { return AllItemTypeTags;}
	uint8 GetEquipmentItemCapaicty(FGameplayTag ItemType); 
	uint8 GetInventoryItemContainerSize() { return ItemContainerSize; }
	AActor* GetSpawnedEquippedActor(FGameplayTag ItemTypeTag) const;
	void ChangeNextWeapon(float ChangedIndex);

	//아이템 장착 및 해제
	void RegisterItemFromInventoryToSlot(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex);
	void UnRegisterItemFromSlot(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex);

	//아이템 장착 여부확인
	bool IsItemEquipped(UItemBaseDataAsset* Item);
	bool IsEquippedItem(FGameplayTag ItemTypeTag, uint8 Index);
protected:
//	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void AddStartingItem();
	void AddItemToItemContainer(const UItemBaseDataAsset* EquipDataAsset);

	//장비 아이템 스폰 및 어빌리티 부여
	void SpawnEquipItemAndEquip(const UEquipBaseItemDataAsset* EquipDataAsset);
	void GrantAbilitiesFromEquipItemData(const UEquipBaseItemDataAsset* AddedItem);


	void RemoveAbilitiesFromEquipItemData(const UEquipBaseItemDataAsset* EquipDataAsset);
public:
	FOnEquipmentChangeDelegate OnEquipmentChange;
protected:

	UPROPERTY(EditDefaultsOnly,Category = "InventoryComponent | Items")
	TArray<TObjectPtr<UItemBaseDataAsset>>	StartingItems;

	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> InventoryItemContainer;

	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> RegisteredItemContainer;

	TMap<FGameplayTag, TMap<int32, int32>> InventoryToQuickSlotMap;

	TMap<FGameplayTag, TMap<int32, int32>> InventoryToEquipSlotMap;

	TMap<FGameplayTag, TObjectPtr<AEquipItem>> EquippedItemActors;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent")
	TMap<FGameplayTag, int32> CurrentQuickSlotIndices;
	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent")
	FGameplayTag CurrentUsingQuickSlotType;

	TMap<FGameplayTag, uint8> ActiveRegisteredItemIndexMap;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	int8 ItemContainerSize{ 25 };

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	TMap<FGameplayTag, uint8> RegisteredItemContainerCapacity;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items")
	FGameplayTagContainer AllItemTypeTags;

	// 어빌리티 핸들을 아이템 기준으로 추적
	UPROPERTY()
	TMap<UEquipBaseItemDataAsset*, FGrantedAbilityHandles> GrantedAbilityMap;
};
