// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GaugeBar.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class ACTIONFRAMEWORK_API UGaugeBar : public UARPGUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void WidgetPresenterSet() override;

	UFUNCTION()
	void UpdateGauge(float Percent);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UProgressBar> ProgressBar;
};
