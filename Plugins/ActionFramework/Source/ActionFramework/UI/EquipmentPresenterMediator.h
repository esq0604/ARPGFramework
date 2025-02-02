// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "EquipmentPresenterMediator.generated.h"

/**
 * 
 */
class UEquipmentPresenter;
class UItemDescriptionPanel;
class UItemListPaenlPresenter;

USTRUCT()
struct FEquipmentSlotClickedMessage
{
	GENERATED_BODY()

	FEquipmentSlotClickedMessage() {}

	FEquipmentSlotClickedMessage(FGameplayTag InItemTypeTag, uint8 InSlotIndex)
		: ItemTypeTag(InItemTypeTag), SlotIndex(InSlotIndex) {}

    FGameplayTag ItemTypeTag;

    uint8 SlotIndex;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotClickedSignature, const FEquipmentSlotClickedMessage&, Msg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotEnteredSignature, const FEquipmentSlotClickedMessage&, Msg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemListSlotEnteredSignature, const FEquipmentSlotClickedMessage&, Msg);

UCLASS(BlueprintType, Blueprintable)
class ACTIONFRAMEWORK_API UEquipmentPresenterMediator : public UObject
{
	GENERATED_BODY()
public:
	
	//void NotifyEquipmentSlotClicked(const FEquipmentSlotClickedMessage& Msg);

	FOnEquipmentSlotClickedSignature OnEquipmentSlotClickedDelegate;
	FOnEquipmentSlotEnteredSignature OnEquipmentSlotEnteredDelegate;
	FOnItemListSlotEnteredSignature OnItemListSlotEnteredDelegate;

	TObjectPtr<UEquipmentPresenter> EquipmentPresenter;
	TObjectPtr<UItemListPaenlPresenter> ItemListPanelPresenter;
};
