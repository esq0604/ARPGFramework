// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "GameplayEffectTypes.h"
#include "OverlayPresenter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate, float, NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossNameChangeDelegate, FName, NewValue);
UCLASS()
class ACTIONFRAMEWORK_API UOverlayPresenter : public UARPGPresenter
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacksToDependencies() override;

	void SetBoss(AActor* NewBoss);

public:
	void PlayerHealthChanged(const FOnAttributeChangeData& Data);
	void PlayerMaxHealthChanged(const FOnAttributeChangeData& Data);
	void PlayerUpdateHealthPercent(float NewHealth, float NewMaxHealth);

	void PlayerPostureChanged(const FOnAttributeChangeData& Data);
	void PlayerMaxPostureChanged(const FOnAttributeChangeData& Data);
	void PlayerUpdatePosturePercent(float NewPosture, float NewMaxPosture);

	void BossHealthChanged(const FOnAttributeChangeData& Data);
	void BossPostureChanged(const FOnAttributeChangeData& Data);
	void BossUpdateHealthPercent(float NewHealth, float NewMaxHealth);
	void BossUpdatePosturePercent(float NewPosture, float NewMaxPosture);


public:
	FOnPercentChangeDelegate OnPlayerHealthPercentChange;
	FOnPercentChangeDelegate OnPlayerPosturePercentChange;

	FOnAttributeChangedSignature OnBossHealthChange;
	FOnAttributeChangedSignature OnBossPostureChange;
	FOnPercentChangeDelegate OnBossHealthPercentChange;
	FOnPercentChangeDelegate OnBossPosturePercentChange;
	FOnBossNameChangeDelegate OnBossNameChange;

private:
	TObjectPtr<AActor> BossActor;
	TObjectPtr<UAbilitySystemComponent> BossASC;
};
