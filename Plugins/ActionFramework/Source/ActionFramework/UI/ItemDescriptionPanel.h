// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGUserWidget.h"
#include "ItemDescriptionPanel.generated.h"

/**
 * 
 */
class UTextBlock;
class UTexture2D;
class UImage;

USTRUCT()
struct FDescriptionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	int Count;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	int RemaningCount;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	FText ItemDes;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	FText ItemEffect;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	UTexture2D* ItemImage;
};

UCLASS()
class ACTIONFRAMEWORK_API UItemDescriptionPanel : public UARPGUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdateDescription(FDescriptionInfo NewDesInfo);
private:
	FDescriptionInfo DesInfo;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> Count;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> RemaningCount;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> ItemDes;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> ItemEffect;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> ItemImage;
};
