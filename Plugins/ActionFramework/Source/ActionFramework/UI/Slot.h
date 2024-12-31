// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slot.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

UENUM()
enum class ESlotType
{
	Inventory,
	Equipment,
	Quick
};

struct FSlotDisplayInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, USlot*, ClickedSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotMouseEnter, USlot*, ClickedSlot);

UCLASS()
class ACTIONFRAMEWORK_API USlot : public UUserWidget
{
	GENERATED_BODY()
public:

//	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	void ClearItemInfo();
	void UpdateItemInfo(const FSlotDisplayInfo& ItemInfo);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotClicked;
	FOnSlotMouseEnter OnSlotMouseEnter;
	//UPROPERTY(EditInstanceOnly)
	//TObjectPtr<UTexture2D> BackgroundIconTexture;
private:
	//UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	//TObjectPtr<UImage> BackgroundIcon;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UImage> DefaultImg;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> AmountText;
};
