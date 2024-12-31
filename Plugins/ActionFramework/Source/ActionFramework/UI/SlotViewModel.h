// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SlotViewModel.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSlotDisplayInfo
{
	GENERATED_BODY()
public:
	FSlotDisplayInfo() = default;

	FSlotDisplayInfo(UTexture2D* InItemIcon, int32 InItemCount, const FText& InItemName)
		: Icon(InItemIcon), Name(InItemName), Count(InItemCount)
	{}
	TObjectPtr<UTexture2D> Icon;

	FText Name;

	int32 Count;

};

UCLASS()
class ACTIONFRAMEWORK_API USlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

	void UpdateItemInfo(const FSlotDisplayInfo& ItemInfo);

	
};
