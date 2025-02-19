// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"
#include "ARPGMeleeAttackAbility.generated.h"

/**
 * 
 */
class UAnimMontage;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitInputPress;
class UAbilityTask_WaitGameplayTagAdded;
class UAbilityTask_WaitGameplayTagRemoved;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask;
class UComboDataAsset;
UCLASS()
class ACTIONFRAMEWORK_API UARPGMeleeAttackAbility : public UARPGAbility
{
	GENERATED_BODY()
public:
	const struct FComboInfo* GetCurrentComboData() const;
	const struct FHitReactionInfo* GetCurrentHitReactionInfo() const;
	
private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

	UFUNCTION()
	void MontageFinish();

	UFUNCTION()
	void MontageCanceled();

	UFUNCTION()
	void MontageInterrupted();

private:
	void Attack(uint8 ComboIndex);

	UFUNCTION()
	void AttackHitEvent(FGameplayEventData Payload);

	UFUNCTION()
	void AddCanNextComboTag();

	UFUNCTION()
	void RemoveCanNextComboTag();

	UFUNCTION()
	void NextAttackInputEvent(float TimeWaited);

	//UFUNCTION()
	//void AttackParryEvent(FGameplayEventData Payload);

	UAbilitySystemComponent* GetAttackHitTargetASC(const AActor* Target);
private:

	TArray<UAbilityTask_WaitGameplayEvent*> AttackHitEvents;
	TMap<UAbilityTask_WaitGameplayEvent*, uint8> AttackEventHitReactionMap;

	UPROPERTY(EditDefaultsOnly, Category = "ComboInfo")
	TObjectPtr<UComboDataAsset> ComboDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "ComboInfo")
	FGameplayTag WaitTag;

	UPROPERTY(EditDefaultsOnly, Category = "ComboInfo")
	FGameplayTag SendHitReceivedTag;

	UPROPERTY(EditDefaultsOnly, Category = "ComboInfo")
	FGameplayTagContainer BlockHitTags;

	UAbilityTask_PlayMontageAndWait* MontgeTask;
	UAbilityTask_WaitInputPress* WaitInputTask;
	UAbilityTask_WaitGameplayTagAdded* AddWaitTagTask;
	UAbilityTask_WaitGameplayTagRemoved* RemoveWaitTagTask;

	uint8 CurrentHitReactionIdex{ 0 };
	uint8 CurrentActivateComboIndex{ 0 };
	uint8 CurrentTryActivateComboIndex{ 0 };
};
