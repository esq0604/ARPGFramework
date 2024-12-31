// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ActionFramework/UI/Slot.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
class UUniformGridPanel;
class UButton;
class USlot;
class UItemListPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentCloseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentRequestItemList, FGameplayTag, ItemTypeTag, UUserWidget*, RequestingWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentRequsetItemName, FGameplayTag, ItemTypeTag, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRequestEquipItem, FGameplayTag, ItemTypeTag, int32, SlotIndex, int32 , EquipSlot);


UCLASS()
class ACTIONFRAMEWORK_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void UpdateItemListPanel(const TArray<FSlotDisplayInfo>& Infos);
	void UpdateEquipRequestSlot(const FSlotDisplayInfo& Info);
	void SetCurOpenItemListPanel(FGameplayTag ItemTypeTag);
private:
	UFUNCTION(Category = "UI")
	void HandleCloseButtonClicked();

	UFUNCTION(Category = "UI")
	void HandleEquipmentSlotClicked(USlot* ClickedSlot);

	UFUNCTION(Category = "UI")
	void HandleItemListSlotClicked(int32 ClickedSlotIndex);

	UFUNCTION(Category = "UI")
	void HandleItemListSlotEntered(int32 EnteredSlotIndex);

	int32 GetSlotIndex(FGameplayTag ItemTypeTag);

public:
	FOnEquipmentCloseButtonClicked OnCloseButtonClicked;
	FOnEquipmentRequestItemList	OnRequestItemList;
	FOnRequestEquipItem OnRequestEquipItem;
	FOnEquipmentRequsetItemName OnRequestItemName;
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UWidget> SlotWrapperPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> WeaponPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> ToolPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ArmorSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> GlovesSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> HelmetSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ShoesSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> CloseButton;

	// Slot-Tag ╦егн©К TMap
	TMap<USlot*, FGameplayTag> SlotTagMap;

	
	TMap<FGameplayTag, UUniformGridPanel*> TagPanelMap;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int8 NumWeaponSlot{ 3 };

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int8 NumToolSlot{ 6 };

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UItemListPanel> ItemListPanel;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 SlotCapacity{ 25 };

	TObjectPtr<USlot> EquipRequestSlot;
	

	FGameplayTag CurOpenItemListPanel;

};
