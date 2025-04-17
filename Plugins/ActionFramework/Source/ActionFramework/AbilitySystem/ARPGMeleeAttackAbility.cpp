// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGMeleeAttackAbility.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Interface/Combatable.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
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
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("ActivateAbility"));
        CurrentActivateComboIndex = CurrentTryActivateComboIndex;

        Attack(CurrentTryActivateComboIndex);

       // GetSourceObject()->

    }

        
}

void UARPGMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    UE_LOG(LogTemp, Warning, TEXT("EndAbility Ability"));

    if (!bWasCancelled)
    {
        CurrentTryActivateComboIndex = 0;
        CurrentHitReactionIdex = 0;
    }

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
    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(ARPGGameplayTags::Status_Attack_CanNextAttack);

    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}

void UARPGMeleeAttackAbility::Attack(uint8 ComboIndex)
{
    if (UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())))
    {
        CurrentActivateComboIndex = ComboIndex;

        MontgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ComboDataAsset->ComboInfos[CurrentTryActivateComboIndex].AttackMontage);
        MontgeTask->OnCompleted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageFinish);
        MontgeTask->OnInterrupted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageInterrupted);
        MontgeTask->OnCancelled.AddDynamic(this, &UARPGMeleeAttackAbility::MontageCanceled);
  
        WaitAttackHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ARPGGameplayTags::GameplayEvent_Attack_Hit, nullptr, false, true);
        WaitAttackHitEventTask->EventReceived.AddDynamic(this, &UARPGMeleeAttackAbility::AttackHitEvent);
        WaitAttackHitEventTask->ReadyForActivation();

        MontgeTask->ReadyForActivation();

        AddWaitTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, WaitTag, nullptr, false);
        AddWaitTagTask->Added.AddDynamic(this, &UARPGMeleeAttackAbility::AddCanNextComboTag);
        AddWaitTagTask->ReadyForActivation();
    }
}

void UARPGMeleeAttackAbility::AttackHitEvent(FGameplayEventData Payload)
{

     if (const UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(Payload.ContextHandle.GetSourceObject()))
     {
         if (!ComboDataAsset->ComboInfos.IsValidIndex(CurrentActivateComboIndex))
         {
             FString ComboIndexDebugText = FString::Printf(TEXT("CurrentActivateComboIndex is not vaild  : %d"), CurrentActivateComboIndex);

             GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, ComboIndexDebugText);

             return;
         }
         if (!ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos.IsValidIndex(CurrentHitReactionIdex))
         {
             FString ComboIndexDebugText = FString::Printf(TEXT("HitReactionInfo is not vaild  : %d"), CurrentHitReactionIdex);

             GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, ComboIndexDebugText);

             //FString HitReactIndexDebugText = FString::Printf(TEXT("AttackAbility is not vaild HitReactIndex : %d"), CurrentHitReactionIdex);
             //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, HitReactIndexDebugText);
             return;
         }

         FString ComboIndexDebugText = FString::Printf(TEXT("CurrentActivateComboIndex Index: %d"), CurrentActivateComboIndex);
         GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, ComboIndexDebugText);

         FString HitReactIndexDebugText = FString::Printf(TEXT("HitReactionInfo Index : %d"), CurrentHitReactionIdex);
         GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, HitReactIndexDebugText);

         FHitReactionInfo CurHitReaction = ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos[CurrentHitReactionIdex];
         TSubclassOf<UGameplayEffect> DamageClass = CurHitReaction.DamageEffect;
         FGameplayTag Direction = CurHitReaction.AttackDirection;
         UAbilitySystemComponent* TargetASC = GetAttackHitTargetASC(Payload.Target);

         //Combo Index를 넘겨주기위해 Custom EffectContext를 제작하빈다.
      
         FGameplayEffectContextHandle ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo());  //GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
         FARPGGameplayEffectContext* ARPGContext = static_cast<FARPGGameplayEffectContext*>(ContextHandle.Get());
         ARPGContext->ComboIndex = CurrentActivateComboIndex;
         ARPGContext->HitReactIndex = CurrentHitReactionIdex;
         ARPGContext->SetAbility(this);
         ARPGContext->AddSourceObject(ComboDataAsset);

         Payload.ContextHandle = ContextHandle;
         if (TargetASC && DamageClass)
         {
             GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("AttackEffect To Target"));
             GetActorInfo().AbilitySystemComponent->ApplyGameplayEffectToTarget(DamageClass.GetDefaultObject(), TargetASC, 0.f, Payload.ContextHandle);
         }
     }

     if (CurrentActivateComboIndex == CurrentTryActivateComboIndex)
     {
         CurrentHitReactionIdex++;
     }
     else
     {
         CurrentHitReactionIdex = 0;
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
        AddWaitTagTask->EndTask();
    }

    if (RemoveWaitTagTask)
    {
        RemoveWaitTagTask->EndTask();
    }

    if (WaitAttackHitEventTask)
    {
        WaitAttackHitEventTask->EndTask();
    }

    if (UComboDataAsset* ComboDataAsset = CastChecked<UComboDataAsset>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())))
    {

        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(ARPGGameplayTags::Status_Attack_CanNextAttack);
        // 다음 콤보 인덱스 설정
        CurrentTryActivateComboIndex = (CurrentTryActivateComboIndex + 1) % ComboDataAsset->ComboInfos.Num();

        //다음 콤보의 히트리엑션 인덱스 재설정
        CurrentHitReactionIdex = 0;

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


