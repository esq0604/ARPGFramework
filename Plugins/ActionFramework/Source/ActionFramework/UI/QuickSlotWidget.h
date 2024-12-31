// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
class USlot;
UCLASS()
class ACTIONFRAMEWORK_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget),Category="UI")
	TObjectPtr<USlot> WeaponSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ToolItemSlot;

};
