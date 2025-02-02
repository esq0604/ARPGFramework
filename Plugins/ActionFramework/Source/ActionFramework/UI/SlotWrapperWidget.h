// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "SlotWrapperWidget.generated.h"

/**
 * 
 */
class UUniformGridPanel;
class USlot;

UCLASS()
class ACTIONFRAMEWORK_API USlotWrapperWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnSlotClicked(USlot* ClickedSlot);
private:
	virtual void WidgetPresenterSet() override;



private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> WeaponPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> ToolPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UUniformGridPanel> ArmorPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ArmorSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> GlovesSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> HelmetSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<USlot> ShoesSlot;
};
