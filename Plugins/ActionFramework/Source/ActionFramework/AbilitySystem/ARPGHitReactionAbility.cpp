// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGHitReactionAbility.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ActionFramework/Datas/ComboDataAsset.h"

UARPGHitReactionAbility::UARPGHitReactionAbility()
{
	FAbilityTriggerData Data;
	Data.TriggerTag = ARPGGameplayTags::GameplayEvent_HitReact;
	AbilityTriggers.Add(Data);

    AbilityTags.AddTag(ARPGGameplayTags::Abilities_HitReact);
}

void UARPGHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !TriggerEventData || !TriggerEventData->ContextHandle.IsValid())
    {
        return;
    }


    FGameplayEffectContextHandle ContextHandle = TriggerEventData->ContextHandle;
    FARPGGameplayEffectContext* EffectContext = static_cast<FARPGGameplayEffectContext*>(ContextHandle.Get());
    if (!EffectContext)
    {
        return;
    }

    UComboDataAsset* ComboDataAsset = Cast<UComboDataAsset>(EffectContext->GetSourceObject());
    if (!ComboDataAsset)
    {
        return;
    }

    //공격 방향 확인
    const FGameplayTag AttackDirectionTag = ComboDataAsset->ComboInfos[EffectContext->ComboIndex].AttackDirection;

    //공격 방향에 따른 HitReaction 재생
    if (UHitReactionComponent* HitReactComp = GetHitReactionComponent(GetOwningActorFromActorInfo()))
    {
        UAnimMontage* HitReactMontage = HitReactComp->GetHitReaction(AttackDirectionTag);
        if (HitReactMontage)
        {
            MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), HitReactMontage);
            MontageTask->OnCompleted.AddDynamic(this, &UARPGHitReactionAbility::MontageFinish);
            MontageTask->OnInterrupted.AddDynamic(this, &UARPGHitReactionAbility::MontageInterrupted);
            MontageTask->OnCancelled.AddDynamic(this, &UARPGHitReactionAbility::MontageCanceled);
            MontageTask->ReadyForActivation();
        }
    }
}

UHitReactionComponent* UARPGHitReactionAbility::GetHitReactionComponent(AActor* OwnerActor) const
{
    return OwnerActor ? Cast<UHitReactionComponent>(OwnerActor->GetComponentByClass(UHitReactionComponent::StaticClass())) : nullptr;
}

void UARPGHitReactionAbility::MontageFinish()
{
    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);

}

void UARPGHitReactionAbility::MontageCanceled()
{
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}

void UARPGHitReactionAbility::MontageInterrupted()
{
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}
