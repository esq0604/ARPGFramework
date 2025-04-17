// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "GaugeBar.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class ACTIONFRAMEWORK_API UGaugeBar : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:	
	UFUNCTION()
	void SetBarPercent(float NewVal);

	void BindPercentDelegate(FOnPercentChangeDelegate& InDelegate);
protected:
	virtual void WidgetPresenterSet() override;
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UProgressBar> ProgressBar;
};
