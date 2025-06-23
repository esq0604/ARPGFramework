// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"
#include "ARPGExecutionAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UARPGExecutionAbility : public UARPGAbility
{
	GENERATED_BODY()
	
public:
	UARPGExecutionAbility();

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void MontageFinish();

	UFUNCTION()
	void MontageCanceled();

	UFUNCTION()
	void MontageInterrupted();
};
