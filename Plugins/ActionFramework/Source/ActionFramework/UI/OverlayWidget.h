// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "OverlayWidget.generated.h"

/**
 * 
 */
class UGaugeBar;

UCLASS()
class ACTIONFRAMEWORK_API UOverlayWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void WidgetPresenterSet() override;
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UGaugeBar> HPGauge;
};
