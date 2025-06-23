// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGExecutionAbility.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UARPGExecutionAbility::UARPGExecutionAbility()
{
	
	FAbilityTriggerData TriggerData;

	TriggerData.TriggerTag = ARPGGameplayTags::GameplayEvent_Execution;
	AbilityTriggers.Add(TriggerData);

	AbilityTags.AddTag(ARPGGameplayTags::Abilities_Execute);
}

void UARPGExecutionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!TriggerEventData || !TriggerEventData->OptionalObject) return;

    const UExecutionDataAsset* ExecutionData = Cast<UExecutionDataAsset>(TriggerEventData->OptionalObject);
    if (!ExecutionData) return;

	UAbilityTask_PlayMontageAndWait* ExecuteAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ExecutionData->ExecutionAnim);

	ExecuteAnimTask->ReadyForActivation();
}

void UARPGExecutionAbility::MontageFinish()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void UARPGExecutionAbility::MontageCanceled()
{

	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);
}

void UARPGExecutionAbility::MontageInterrupted()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);
}
