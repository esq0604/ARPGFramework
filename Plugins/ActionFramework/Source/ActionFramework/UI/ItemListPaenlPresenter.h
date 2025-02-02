// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "GameplayTagContainer.h"
#include "ItemListPaenlPresenter.generated.h"

class UEquipmentPresenterMediator;
struct FEquipmentSlotClickedMessage;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChangeSignature, uint8, Index, const FSlotDisplayInfo&, SlotInfo);

UCLASS()
class ACTIONFRAMEWORK_API UItemListPaenlPresenter : public UARPGPresenter
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacksToDependencies() override;

	FGameplayTag GetCurItemListType() { return CurItemListType; }

	UFUNCTION()
	void OnItemListSlotEntered(const FEquipmentSlotClickedMessage& Msg);
private:
	void UpdateItemListPanel(FGameplayTag ItemType);

	UFUNCTION()
	void OnItemListPanelUpdate(const FEquipmentSlotClickedMessage& Msg);

	UFUNCTION()
	void EquipUnEquipItem(FGameplayTag ItemType, uint8 InventoryListClickedSlotIndex);
public:
	//FOnEquipItemRequestSinature OnEquipItemRequestDelegate;
	FOnEquipmentChangeSignature OnEquipmentChangeDelegate;
private:
	//TObjectPtr<UEquipmentPresenterMediator> PresenterMediator;
	FGameplayTag CurItemListType;
	uint8 CurClickedCatogoryIndex;
};
