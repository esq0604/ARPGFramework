// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "OverlayPresenter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeDelegate, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeDelegate, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate, float, NewPercent);


struct FOnAttributeChangeData;

UCLASS()
class ACTIONFRAMEWORK_API UOverlayPresenter : public UARPGPresenter
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacksToDependencies() override;

private:
	void HealthChanged(const FOnAttributeChangeData& Data);
	void MaxHealthChanged(const FOnAttributeChangeData& Data);

	void UpdateHealthPercent(float NewHealth, float NewMaxHealth);
public:
	FOnHealthChangeDelegate OnHealthChange;
	FOnMaxHealthChangeDelegate OnMaxHealthChange;
	FOnPercentChangeDelegate OnHealthPercentChange;
private:

};
