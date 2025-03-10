// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGHitReactionAbility.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
UARPGHitReactionAbility::UARPGHitReactionAbility()
{
	FAbilityTriggerData Data;
	Data.TriggerTag = ARPGGameplayTags::GameplayEvent_HitReact;
	AbilityTriggers.Add(Data);
}

void UARPGHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		if (TriggerEventData->ContextHandle.IsValid())
		{
			FGameplayEffectContextHandle ContextHandle = TriggerEventData->ContextHandle;
			FARPGGameplayEffectContext* EffectContext = static_cast<FARPGGameplayEffectContext*>(ContextHandle.Get());

			FString ComboIndexDebugText = FString::Printf(TEXT("ARPGEffectContext Get ComboIndex : %d"), EffectContext->ComboIndex);
			FString HitReactIndexDebugText = FString::Printf(TEXT("ARPGEffectContext Get HitReactIndex : %d"), EffectContext->HitReactIndex);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, ComboIndexDebugText);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, HitReactIndexDebugText);
		}
	}
}
