// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GameplayTagContainer.h"
#include "Slot.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UserWidget;
struct FSlotDisplayInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, USlot*, ClickedSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotMouseEnter, USlot*, ClickedSlot);

UCLASS()
class ACTIONFRAMEWORK_API USlot : public UARPGUserWidget
{
	GENERATED_BODY()
public:

//	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	void ClearSlot();
	void UpdateSlot(const FSlotDisplayInfo& ItemInfo);
	void SetSlotType(FGameplayTag TypeTag);

	FORCEINLINE FGameplayTag GetSlotType() { return SlotType; }
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotClicked;
	FOnSlotMouseEnter OnSlotMouseEnter;

	//UPROPERTY(EditInstanceOnly)
	//TObjectPtr<UTexture2D> BackgroundIconTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> EquipCheck;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> AmountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "UI")
	FGameplayTag SlotType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UImage> DefaultImg;

	UPROPERTY(EditAnywhere, Category = "UI")
	uint8 SlotIndex;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText SlotNameText;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText StoreItemText;
protected:
	//UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	//TObjectPtr<UImage> BackgroundIcon;


};
