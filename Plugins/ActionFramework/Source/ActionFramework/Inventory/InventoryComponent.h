// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "InventoryComponent.generated.h"

class UItemBaseDataAsset;
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
	const UItemBaseDataAsset* GetEquipmentItem(FGameplayTag ItemType, uint8 Index);
	bool IsItemEquipped(UItemBaseDataAsset* Item);
	UItemBaseDataAsset* GetCurrentEquipWeaponData();
	
	FGameplayTagContainer GetAllItemType() { return AllItemTypeTags;}
	uint8 GetEquipmentItemCapaicty(FGameplayTag ItemType); 
	uint8 GetInventoryItemContainerSize() { return ItemContainerSize; }
	void ChangeNextWeapon(float ChangedIndex);
	void EquipItemFromInventoryItemContainer(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex);
	void UnEquipItem(FGameplayTag ItemTypeTag, uint8 RequsetItemIndex, uint8 UpdateSlotIndex);
	bool IsEquippedItem(FGameplayTag ItemTypeTag, uint8 Index);
protected:
//	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void AddStartingItem();
	void AddItemToItemContainer(const UItemBaseDataAsset* AddedItem);

	UFUNCTION(Category = "InventoryComponent")
	void EquipUnEquipItemToEquipment(FGameplayTag ItemType, int32 ContainerIndex, int32 EquipIndex);

public:
	FOnEquipmentChangeDelegate OnEquipmentChange;
protected:

	UPROPERTY(EditDefaultsOnly,Category = "InventoryComponent | Items")
	TArray<TObjectPtr<UItemBaseDataAsset>>	StartingItems;

	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> InventoryItemContainer;

	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> EquipmentItemContainer;

	TMap<FGameplayTag, TMap<int32, int32>> InventoryToQuickSlotMap;

	TMap<FGameplayTag, TMap<int32, int32>> InventoryToEquipSlotMap;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent")
	TMap<FGameplayTag, int32> CurrentQuickSlotIndices;
	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent")
	FGameplayTag CurrentUsingQuickSlotType;

	TMap<FGameplayTag, uint8> CurUsingTagEquipmentContainerIndexMap;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	int8 ItemContainerSize{ 25 };

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	TMap<FGameplayTag, uint8> EquipmentItemContainerCapacity;

	/*UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	uint8 EquipWeaponCapacity{ 3 };

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items | Capacity")
	uint8 EquipToolCapacity{ 6 };*/

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | Items")
	FGameplayTagContainer AllItemTypeTags;

//
//
//
//	UPROPERTY()
//	FEquipItems EquipWeaponItems;
//
//	TMap<EItemType, FEquipItems> CurEquipItems;
//
//	//아이템 타입에 따른 인벤토리와 퀵슬롯의 인덱스를 매핑시켜주도록 합니다.


		
};
