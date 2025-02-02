// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GameplayTagContainer.h"
#include "ActionFramework/UI/Slot.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
class UUniformGridPanel;
class UItemDescriptionPanel;
class UButton;
class USlot;
class UItemListPanel;
class USlotWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentCloseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategorySlotClickedSignature, FGameplayTag, ItemTypeTag, uint8, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategorySlotEnteredSignature, FGameplayTag, ItemTypeTag, uint8, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRequestEquipItem, FGameplayTag, ItemTypeTag, int32, SlotIndex, int32 , EquipSlot);


UCLASS()
class ACTIONFRAMEWORK_API UEquipmentWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void ShowItemListPanel();
private:

	UFUNCTION()
	void UpdateSlot(FGameplayTag ItemTypeTag, uint8 Index, const FSlotDisplayInfo& SlotInfo);

	UFUNCTION()

	virtual void WidgetPresenterSet();

	UFUNCTION(Category = "UI")
	void HandleCloseButtonClicked();

	UFUNCTION(Category = "UI")
	void OnCategorySlotClicked(USlot* ClickedSlot);

	UFUNCTION(Category = "UI")
	void OnCategorySlotEntered(USlot* EnteredSlot);

	void InitSlotInPanel(UUniformGridPanel* SlotPanel);

	UFUNCTION()
	void UpdateSlotNameTextBlock(FGameplayTag ItemTypeTag, uint8 Index);

	UFUNCTION()
	void UpdateStoreItemNameTextBlock(FGameplayTag ItemTypeTag, uint8 Index);

	USlot* FindSlotAtSlotTagMap(FGameplayTag ItemTypeTag, uint8 Index);
	//int32 GetSlotIndex(FGameplayTag ItemTypeTag);*/

public:
	FOnEquipmentCloseButtonClicked OnCloseButtonClicked;
	FOnCategorySlotClickedSignature OnCategorySlotClickedDelegate;
	FOnCategorySlotEnteredSignature OnCategorySlotEnteredDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UItemListPanel> ItemListPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UWidget> ItemCategoryPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UItemDescriptionPanel> ItemInfoPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UWidget> CharacterDataPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> WeaponPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> ToolPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> ArmorPanel; 

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ArmorSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> HelmetSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> GloveSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ShoesSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> CloseButton;

	// Slot-Tag ╦егн©К TMap
	TMap<FGameplayTag, TMap<uint8 ,TObjectPtr<USlot>>> SlotTagMap;

	TMap<FGameplayTag, UUniformGridPanel*> TagPanelMap;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 SlotCapacity{ 25 };

	TObjectPtr<USlot> EquipRequestSlot;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> SlotName_TextBlock;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> StoreItemName_TextBlock;

	FGameplayTag CurOpenItemListPanel;

};
