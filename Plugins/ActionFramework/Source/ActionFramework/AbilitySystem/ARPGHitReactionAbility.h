// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"
#include "ARPGHitReactionAbility.generated.h"

/**
 * 
 */
class UHitReactionComponent;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class ACTIONFRAMEWORK_API UARPGHitReactionAbility : public UARPGAbility
{
	GENERATED_BODY()

public:
	UARPGHitReactionAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UHitReactionComponent* GetHitReactionComponent(AActor* OwnerActor) const;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UFUNCTION()
	void MontageFinish();

	UFUNCTION()
	void MontageCanceled();

	UFUNCTION()
	void MontageInterrupted();
};
