// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ActionFramework/UI/Slot.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UButton;
class UItemListPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryOnCloseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryRequestItemList, FGameplayTag, ItemTypeTag, UUserWidget*, RequestingWidget);
UCLASS()
class ACTIONFRAMEWORK_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	//void UpdateItemListPanel(const TArray<FSlotDisplayInfo>& Infos);
	void SetCurOpenItemListPanel(FGameplayTag ItemTypeTag);

private:
	UFUNCTION(Category = "UI")
	void HandleButtonClicked(FGameplayTag ItemTypeTag);

	UFUNCTION(Category = "UI")
	void HandleCloseButtonClicked();

	UFUNCTION(Category = "UI")
	void OnWeaponButtonClicked();

	UFUNCTION(Category = "UI")
	void OnToolButtonClicked();

	UFUNCTION(Category = "UI")
	void OnHelmetButtonClicked();

	UFUNCTION(Category = "UI")
	void OnArmorButtonClicked();

	UFUNCTION(Category = "UI")
	void OnGlovesButtonClicked();

	UFUNCTION(Category = "UI")
	void OnShoesButtonClicked();
public:
	FInventoryOnCloseButtonClicked OnCloseButtonClicked;
	FOnInventoryRequestItemList OnItemTypeSelected;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> WeaponButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ToolButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HelmetButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ArmorButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GlovesButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ShoesButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UItemListPanel> ItemListPanel;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 SlotCapacity{ 25 };

	FGameplayTag CurOpenItemListPanel;

};
