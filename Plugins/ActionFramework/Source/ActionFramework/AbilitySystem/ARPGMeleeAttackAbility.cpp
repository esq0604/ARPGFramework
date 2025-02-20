// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGMeleeAttackAbility.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Interface/Combatable.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"

#include "NiagaraFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"


void UARPGMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        CurrentActivateComboIndex = CurrentTryActivateComboIndex;

        Attack(CurrentTryActivateComboIndex);

       // GetSourceObject()->

    }

        
}

void UARPGMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    UE_LOG(LogTemp, Warning, TEXT("EndAbility Ability"));

    if(!bWasCancelled)
        CurrentTryActivateComboIndex = 0;

}

void UARPGMeleeAttackAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    UE_LOG(LogTemp, Warning, TEXT("Cancel Ability"));
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UARPGMeleeAttackAbility::MontageFinish()
{
   // EndAllActivaTasks();

    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),GetCurrentActivationInfoRef(), true, false);
}

void UARPGMeleeAttackAbility::MontageCanceled()
{
    //// 강제로 태그와 Task 초기화
   // EndAllActivaTasks();
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);
}

void UARPGMeleeAttackAbility::MontageInterrupted()
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("MontageInterupt"));
    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Attack.CanNextAttack"));

    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}

void UARPGMeleeAttackAbility::Attack(uint8 ComboIndex)
{
    if (UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())))
    {
        MontgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ComboDataAsset->ComboInfos[CurrentTryActivateComboIndex].AttackMontage);
        MontgeTask->OnCompleted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageFinish);
        MontgeTask->OnInterrupted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageInterrupted);
        MontgeTask->OnCancelled.AddDynamic(this, &UARPGMeleeAttackAbility::MontageCanceled);


        for (uint8 i = 0; i < ComboDataAsset->ComboInfos[ComboIndex].HitReactionInfos.Num(); ++i)
        {
            UAbilityTask_WaitGameplayEvent* AttackHitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ARPGGameplayTags::GameplayEvent_Attack_Hit, nullptr, false, true);
            AttackHitEvent->EventReceived.AddDynamic(this, &UARPGMeleeAttackAbility::AttackHitEvent);
            AttackHitEvent->ReadyForActivation();
            AttackEventHitReactionMap.Add(AttackHitEvent, i);
            AttackHitEvents.Add(AttackHitEvent);
        }

        MontgeTask->ReadyForActivation();

        AddWaitTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, WaitTag, nullptr, false);
        AddWaitTagTask->Added.AddDynamic(this, &UARPGMeleeAttackAbility::AddCanNextComboTag);
        AddWaitTagTask->ReadyForActivation();
    }
}

void UARPGMeleeAttackAbility::AttackHitEvent(FGameplayEventData Payload)
{
     
     if (AttackHitEvents.IsEmpty())
     {
         return;
     }

     if (!AttackEventHitReactionMap.Contains(AttackHitEvents[0]))
     {
         return;
     }

     CurrentHitReactionIdex = AttackEventHitReactionMap[AttackHitEvents[0]];

     if (UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(Payload.ContextHandle.GetSourceObject()))
     {
         if (!ComboDataAsset->ComboInfos.IsValidIndex(CurrentActivateComboIndex))
         {
             return;
         }
         if (!ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos.IsValidIndex(CurrentHitReactionIdex))
         {
             return;
         }
     
    
         FHitReactionInfo CurHitReaction = ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos[CurrentHitReactionIdex];
         TSubclassOf<UGameplayEffect> DamageClass = CurHitReaction.DamageEffect;
         FGameplayTag Direction = CurHitReaction.AttackDirection;
         UAbilitySystemComponent* TargetASC = GetAttackHitTargetASC(Payload.Target);

         Payload.ContextHandle.SetAbility(this);
         Payload.ContextHandle.AddSourceObject(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));
         if (TargetASC && DamageClass)
         {
             //GetActorInfo().AbilitySystemComponent->GameplayEvent
             GetActorInfo().AbilitySystemComponent->ApplyGameplayEffectToTarget(DamageClass.GetDefaultObject(), TargetASC, 0.f, Payload.ContextHandle);
         }
     }


   
}

void UARPGMeleeAttackAbility::AddCanNextComboTag()
{
    if (AddWaitTagTask)
    {
        AddWaitTagTask->EndTask();
    }

    WaitInputTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
    WaitInputTask->OnPress.AddDynamic(this, &UARPGMeleeAttackAbility::NextAttackInputEvent);
    WaitInputTask->ReadyForActivation();

    RemoveWaitTagTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, WaitTag, nullptr, false);
    RemoveWaitTagTask->Removed.AddDynamic(this, &UARPGMeleeAttackAbility::RemoveCanNextComboTag);
    RemoveWaitTagTask->ReadyForActivation();

}

void UARPGMeleeAttackAbility::RemoveCanNextComboTag()
{
    if (WaitInputTask)
    {
        WaitInputTask->EndTask();
    }

    if (AddWaitTagTask)
    {
        AddWaitTagTask->EndTask();
    }
}

void UARPGMeleeAttackAbility::NextAttackInputEvent(float TimeWaited)
{
    if (MontgeTask)
    {
        MontgeTask->EndTask();
    }

    if (WaitInputTask)
    {
        WaitInputTask->EndTask();
    }

    if (AddWaitTagTask)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputTaskBinding :: AddWaitTagEndTask"));
        AddWaitTagTask->EndTask();
    }

    if (RemoveWaitTagTask)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputTaskBinding :: RemoveWaitTagEndTask"));
        RemoveWaitTagTask->EndTask();
    }

    if (UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())))
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Attack.CanNextAttack"));
        // 다음 콤보 인덱스 설정
        CurrentTryActivateComboIndex = (CurrentTryActivateComboIndex + 1) % ComboDataAsset->ComboInfos.Num();

        // 다음 애니메이션 실행
        Attack(CurrentTryActivateComboIndex);
    }
}

//void UARPGMeleeAttackAbility::AttackParryEvent(FGameplayEventData Payload)
//{
//    UAbilityTask_PlayMontageAndWait* ParryMontgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ComboDataAsset->ComboInfos[CurrentActivateComboIndex].DeflectedMontage);
//    ParryMontgeTask->ReadyForActivation();    
//}

UAbilitySystemComponent* UARPGMeleeAttackAbility::GetAttackHitTargetASC(const AActor* Target)
{
    const IAbilitySystemInterface* TargetASCI = Cast<IAbilitySystemInterface>(Target);
    UAbilitySystemComponent* ReturnValue =nullptr;
    if (TargetASCI)
    {
        ReturnValue = TargetASCI->GetAbilitySystemComponent();
    }
    else
    {
        UActorComponent* AC = Target->GetComponentByClass(UAbilitySystemComponent::StaticClass());
        if (AC)
        {
            ReturnValue = Cast<UAbilitySystemComponent>(AC);
        }
    }

    return ReturnValue;
}


