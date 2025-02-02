// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "ItemDescriptionPanelPresenter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UItemDescriptionPanelPresenter : public UARPGPresenter
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallBacksToDependencies() override;

private:
	UFUNCTION()
	void UpdateInventoryItemInfoPanel(const FEquipmentSlotClickedMessage& Msg);

	UFUNCTION()
	void UpdateEquipmentItemInfoPanel(const FEquipmentSlotClickedMessage& Msg);
};
