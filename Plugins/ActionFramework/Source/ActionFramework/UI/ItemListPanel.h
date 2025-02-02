// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GameplayTagContainer.h"
#include "ItemListPanel.generated.h"

/**
 * 
 */
class USlot;
class UUniformGridPanel;
struct FGameplayTag;
struct FSlotDisplayInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemListSlotClickedSignature, FGameplayTag, ItemTypeTag, uint8, SlotIndex);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemListSlotRequsetItemName, int32, EnteredSlotIndex);
UCLASS()
class ACTIONFRAMEWORK_API UItemListPanel : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void InitializeSlot(uint8 Size/*, const TArray<FSlotDisplayInfo>& SlotsInfos, FGameplayTag ItemTypeTag*/);

	void ClearSlots();
	void UpdateSlot(int32 SlotIndex, const FSlotDisplayInfo& ItemInfo);
	UFUNCTION()
	void UpdateSlots(const TArray<FSlotDisplayInfo>& SlotsInfos, uint8 Num);
private:
	UFUNCTION()
	void OnSlotClicked(USlot* ClickedSlot);

	UFUNCTION()
	void OnSlotMouseEntered(USlot* EnteredSlot);

	

	virtual void WidgetPresenterSet() override;
public:
	FOnItemListSlotClickedSignature OnItemListSlotClickedDelegate;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UUniformGridPanel> UniformGridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	TArray<TObjectPtr<UUserWidget>> SlotPool;  // 슬롯 풀

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> SlotTexture;

	UPROPERTY(EditDefaultsOnly)
	int32 NumColumns {5};  // 그리드의 열 개수


};
