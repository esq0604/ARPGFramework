// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "EnemyStatusWidget.generated.h"

/**
 * 
 */
class UGaugeBar;


UCLASS()
class ACTIONFRAMEWORK_API UEnemyStatusWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	

protected:
	void ChangeVisibility();

	virtual void WidgetPresenterSet() override;

	UFUNCTION()
	void OnEnemyHealthChanged(float NewVal);

	UFUNCTION()
	void OnEnemyPostureChanged(float NewVal);

	UFUNCTION()
	void OnEnemyMaxHealthChanged(float NewVal);

	UFUNCTION()
	void OnEnemyMaxPostureChanged(float NewVal);

	UFUNCTION()
	void StatusBarGaugeVisibilityChange(float NewVal);


	void SetBarPercent(UGaugeBar* Widget, float Val , float MaxVal);
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UGaugeBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UGaugeBar> PostureBar;

	FTimerHandle StatusBarHideTimerHandle;

	float Health;
	float MaxHealth;
	float Posture;
	float MaxPosture;

		
};
