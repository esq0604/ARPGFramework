// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GameplayTagContainer.h"
#include "EnemyStatusWidget.generated.h"

/**
 * 
 */
class UGaugeBar;
struct FGameplayTag;

UCLASS()
class ACTIONFRAMEWORK_API UEnemyStatusWidget : public UARPGUserWidget
{
	GENERATED_BODY()
public:
	UEnemyStatusWidget();

	virtual void NativeConstruct() override;

	void SetTargetingState(bool bTargeting);

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

	void HidePostureStatusBar();
	void HideHealthBar();

	void SetBarPercent(UGaugeBar* Widget, float Val , float MaxVal);
	void SetStatusBarState(const FGameplayTag& NewState);
	void UpdateStatusBarVisibility();
	void NotifyDamaged();
	void TryHideStatusBar();
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UGaugeBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UGaugeBar> PostureBar;

	FTimerHandle StatusBarHideTimerHandle;

	FGameplayTag CurrentState;

	bool StatusBarHidden;
	UPROPERTY(EditDefaultsOnly)
	float StatusBarAutoHideDelay = 3.f;

	float Health;
	float MaxHealth;
	float Posture;
	float MaxPosture;

		
};
