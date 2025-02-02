// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "GameplayTagContainer.h"
#include "EquipmentPresenter.generated.h"

/**
 * 
 */
class UImage;
struct FGameplayTag;
class UItemBaseDataAsset;
class UEquipmentPresenterMediator;

USTRUCT(BlueprintType)
struct FSlotDisplayInfo
{
	GENERATED_BODY()
public:
	FSlotDisplayInfo() = default;

	FSlotDisplayInfo(UTexture2D* InItemIcon, int32 InItemCount, const FText& InItemName, bool IsEquipped)
		: Icon(InItemIcon), Name(InItemName), Count(InItemCount), bIsEquipped(IsEquipped)
	{}
	TObjectPtr<UTexture2D> Icon{ nullptr };

	FText Name{ FText::FromString(TEXT("-")) };

	int32 Count{ 0 };

	bool bIsEquipped{ false };

};

/**
 * 슬롯 갱신 시, 해당 정보를 뷰(Widget) 등에게 알리기 위한 Delegate
 * 예: 한 번에 여러 슬롯 정보(FSlotDisplayInfo)를 보낼 수 있게 OneParam에 배열 사용
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategorySlotMouseEnteredDelegate, FGameplayTag, ItemType, uint8, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipItemDelegate, FGameplayTag, ItemTypeTag, uint8, Index, const FSlotDisplayInfo&, SlotInfo);

UCLASS()
class ACTIONFRAMEWORK_API UEquipmentPresenter : public UARPGPresenter
{
	GENERATED_BODY()
	
public:
	UEquipmentPresenter();

	virtual void BroadcastInitialValues() override;
	virtual void BindCallBacksToDependencies() override;
	
	UFUNCTION()
	void OnCategorySlotClicked(FGameplayTag ItemType,  uint8 CategorySlotClickedIndex); //ShowItemList
	UFUNCTION()
	void OnCategorySlotEntered(FGameplayTag ItemType, uint8 EnteredSlotIndex);

	void OnItemListSlotClicked(FGameplayTag ItemType, uint8 InventoryListClickedSlotIndex); //EquipItem
	void OnItemListSlotEntered(FGameplayTag ItemType, uint8 EnteredSlotIndex);

	void SetCurClickedCategorySlotIndex(uint8 Idx);

	UFUNCTION()
	void OnItemEquiped(FGameplayTag RequestTypeTag, uint8 Index, const UItemBaseDataAsset* Item);

public:
	//FOnEquipmentSlotInitDelegete OnEquipmentSlotInit;
	FOnCategorySlotMouseEnteredDelegate OnCategorySlotMouseEntered;
	FOnEquipItemDelegate OnEquipItemEvent;
private:
	uint8 CurClickedCategorySlotIndex;

	

};
