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
class UEquipmentViewModel;

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONFRAMEWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
//	virtual void InitializeComponent() override;

	UFUNCTION(Category = "InventoryComponent")
	void ToggleInventoryWidget();
		//
	UFUNCTION(Category = "InventoryComponent")
	void ToggleEquipmentWidget();

	UFUNCTION(Category = "InventoryComponent")
	void ToggleQuickSlotWidget();

	UItemBaseDataAsset* GetCurrentEquipWeaponData();

//	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void AddStartingItem();
	void AddItemToItemContainer(const UItemBaseDataAsset* AddedItem);
	void AddItemToQuickSlotWidget(const UItemBaseDataAsset* AddedItem);
	void EquipItem(const UItemBaseDataAsset* AddedItem);
	void UnEquipItem(const UItemBaseDataAsset* AddedItem);
	void UseItem(const UItemBaseDataAsset* AddedItem);
	void SetMouseCursorVisibility(bool bIsVisible);

	UFUNCTION(Category = "InventoryComponent")
	void EquipUnEquipItemToEquipment(FGameplayTag ItemType, int32 ContainerIndex, int32 EquipIndex);
	UFUNCTION(Category = "InventoryComponent")
	void UpdateItemList(FGameplayTag ItemTypeTag , UUserWidget* RequestingWidget);

	UFUNCTION(Category = "InventoryComponent")
	void UpdateEquipmentWiget(FGameplayTag ItemTypeTag);
private:
	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | UI")
	TSubclassOf<UUserWidget> EquipmentWidgetClass;

	TObjectPtr<UEquipmentWidget> EquipmentWidget;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent | UI")
	TSubclassOf<UUserWidget> QuickSlotWidgetClass;

	TObjectPtr<UQuickSlotWidget> QuickSlotWidget;

	UPROPERTY(EditDefaultsOnly , Category = "InventoryComponent | StartingItems")
	TArray<TObjectPtr<UItemBaseDataAsset>>	StartingItems;

	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> ItemContainer;
	TMap<FGameplayTag, TArray<TObjectPtr<UItemBaseDataAsset>>> CurEquipedItemData;

	TMap<FGameplayTag, TMap<int32, int32>> InventoryToQuickSlotMap;
	TMap<FGameplayTag, TMap<int32, int32>> InventoryToEquipSlotMap;
	TMap<FGameplayTag, int32> CurrentQuickSlotIndices;
	FGameplayTag CurrentUsingQuickSlotType;

	
	uint8 CurUsingWeaponIndex{ 0 };

	UPROPERTY(EditDefaultsOnly, Category = "InventoryComponent")
	int8 ItemContainerSize{ 25 };

	//TObjectPtr<UEquipmentViewModel> EquipmentViewModel;

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
