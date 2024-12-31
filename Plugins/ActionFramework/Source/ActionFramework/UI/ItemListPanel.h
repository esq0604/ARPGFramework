// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListPanel.generated.h"

/**
 * 
 */
class USlot;
class UUniformGridPanel;
struct FSlotDisplayInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemListSlotClicked, int32 , ClickedSlotIndex);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemListSlotRequsetItemName, int32, EnteredSlotIndex);
UCLASS()
class ACTIONFRAMEWORK_API UItemListPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitializeSlot(int32 TotalSlots);
	void ClearSlots();
	void UpdateSlot(int32 SlotIndex, const FSlotDisplayInfo& ItemInfo);

private:
	UFUNCTION()
	void OnSlotClicked(USlot* ClickedSlot);

	UFUNCTION()
	void OnSlotMouseEntered(USlot* EnteredSlot);
public:
	FOnItemListSlotClicked OnItemListSlotClicked;
	//FOnItemListSlotRequsetItemName OnItemListSlotEntered;

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
