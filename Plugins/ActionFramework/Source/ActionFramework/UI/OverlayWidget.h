// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "OverlayWidget.generated.h"

/**
 * 
 */
class UGaugeBar;
class UTextBlock;
UCLASS()
class ACTIONFRAMEWORK_API UOverlayWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowBossUI();
protected:
	virtual void WidgetPresenterSet() override;

	UFUNCTION()
	void PlayerPostureGaugeVisibilityChange(float Percent);
	UFUNCTION()
	void BossHealthGaugeVisibilityChange(float Percent);
	UFUNCTION()
	void BossPostureGaugeVisibilityChange(float Percent);

	void HidePlayerPostureGauge();
	void HideBossPostureGauge();
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UGaugeBar> PlayerHealthGauge;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UGaugeBar> PlayerPostureGauge;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UGaugeBar> BossHealthGauge;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UGaugeBar> BossPostureGauge;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> BossName;

	FTimerHandle PlayerPostureHideTimerHandle;
	FTimerHandle BossPostureHideTimerHandle;

	bool bPlayerPostureHidden = false;
	bool bBossPostureHidden = false;

	UPROPERTY(EditDefaultsOnly , Category = "UI")
	float PostureAutoHideDelay = 3.f;

};
