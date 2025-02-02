// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGMeleeAttackAbility.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/Interface/Combatable.h"
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

	//if (!ActorInfo->AbilitySystemComponent.IsValid() || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	//{

 //       UE_LOG(LogTemp, Warning, TEXT("ASC, Commit Ability not valid"));
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//}

 //   if (ActorInfo->OwnerActor == nullptr || ComboDataAsset ==nullptr)
 //   {

 //       UE_LOG(LogTemp, Warning, TEXT("OwnerActor null , ComboDataAsset"));
 //       return;
 //   }
 //   if (ComboDataAsset->ComboInfos.Num() == 0)
 //   {
 //       UE_LOG(LogTemp, Warning, TEXT("ComboInfos in ComboDataAsset Num == 0"));
 //       return;
 //   }
 //   if (!ComboDataAsset->ComboInfos.IsValidIndex(CurrentComboIndex))
 //   {
 //       UE_LOG(LogTemp, Warning, TEXT("ComboInfos CurrentComboIndex in ComboDataAsset is not vaild"));
 //       return;
 //   }

    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        CurrentActivateComboIndex = CurrentComboIndex;
        Attack(CurrentComboIndex);
    }
}

void UARPGMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    UE_LOG(LogTemp, Warning, TEXT("EndAbility Ability"));

    if(!bWasCancelled)
    CurrentComboIndex = 0;

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

    MontgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ComboDataAsset->ComboInfos[CurrentComboIndex].AttackMontage);
    MontgeTask->OnCompleted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageFinish);
    MontgeTask->OnInterrupted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageInterrupted);
    MontgeTask->OnCancelled.AddDynamic(this, &UARPGMeleeAttackAbility::MontageCanceled);


    for (uint8 i = 0; i < ComboDataAsset->ComboInfos[ComboIndex].HitReactionInfos.Num(); i++)
    {
        UAbilityTask_WaitGameplayEvent* AttackHitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Attack.Hit"), nullptr, false, true);
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

     uint8 Index = AttackEventHitReactionMap[AttackHitEvents[0]];

     if (!ComboDataAsset->ComboInfos.IsValidIndex(CurrentActivateComboIndex))
     {
         return;
     }
     if (!ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos.IsValidIndex(Index))
     {
         return;
     }
     UAbilityTask_WaitGameplayEvent* ParryEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("State.Block.Parry"), nullptr, true, true);
     ParryEvent->EventReceived.AddDynamic(this, &UARPGMeleeAttackAbility::AttackParryEvent);
     ParryEvent->ReadyForActivation();

     FHitReactionInfo CurHitReaction = ComboDataAsset->ComboInfos[CurrentActivateComboIndex].HitReactionInfos[Index];
     TSubclassOf<UGameplayEffect> DamageClass = CurHitReaction.DamageEffect;
     FGameplayTag Direction = CurHitReaction.AttackDirection;
     UAbilitySystemComponent* TargetASC = GetAttackHitTargetASC(Payload.Target);

     if (TargetASC && DamageClass)
     {
         GetActorInfo().AbilitySystemComponent->ApplyGameplayEffectToTarget(DamageClass.GetDefaultObject(), TargetASC,0.f,Payload.ContextHandle);
     }
     
     //공격로직에서 상대의 방어로직까지 신경쓰는것이 아닌, 공격만 하도록 수정합니다
     //if (TargetASC)
     //{
     //    //ApplyGameplayEffectSpecToTarget()
     //    //타겟이 방어나, 패리 태그를 갖고있다면
     //    //히트 이벤트를 보낸다. 이때 적이 패리태그시 AttackDeflect를 수행하게된다.
     //    if (TargetASC->HasAnyMatchingGameplayTags(BlockHitTags))
     //    {
     //    
     //        UE_LOG(LogTemp, Warning, TEXT("Target Has State.Block Tag"));
     //        FGameplayEventData EventData;
     //        EventData.Instigator = GetAvatarActorFromActorInfo();
     //        EventData.EventTag = CurHitReaction.AttackDirection;
     //        
     //      
     //        AActor* Target = static_cast<AActor*>(Payload.Target);
     //        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, SendHitReceivedTag, EventData);
     //    }

     //    //아니면 그냥 히트 이벤트가 없이 그냥 때린다.
     //    else if (TargetASC && DamageClass != nullptr)
     //    {
     //        GetActorInfo().AbilitySystemComponent->ApplyGameplayEffectToTarget(DamageClass.GetDefaultObject(), TargetASC);


     //        UHitReactionComponent* TargetHitReactionComp = Cast<UHitReactionComponent>(Payload.Target.Get()->GetComponentByClass(UHitReactionComponent::StaticClass()));
     //        if (TargetHitReactionComp)
     //        {
     //            TargetHitReactionComp->ExecuteHitReaction(Direction);
     //        }

     //        const UAttributeSet* At = TargetASC->GetAttributeSet(UARPGAttributeSet::StaticClass());
     //        if (At)
     //        {
     //            //UE_LOG(LogTemp,Warning,TEXT(""))
     //            const UARPGAttributeSet* TargetAttribute = Cast<UARPGAttributeSet>(At);
     //            if (TargetAttribute->GetHealth() <= 0.f)
     //            {
     //                if (CurHitReaction.ExecutionInfo->ExecutionAnim)
     //                {
     //                    UAbilityTask_PlayMontageAndWait* MontageProxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), CurHitReaction.ExecutionInfo->ExecutionAnim);
     //                    MontageProxy->OnCompleted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageFinish);
     //                    MontageProxy->OnInterrupted.AddDynamic(this, &UARPGMeleeAttackAbility::MontageInterrupted);
     //                    MontageProxy->OnCancelled.AddDynamic(this, &UARPGMeleeAttackAbility::MontageCanceled);
     //                    UMotionWarpingComponent* MotionWarpComp = Cast<UMotionWarpingComponent>(GetActorInfo().AvatarActor->GetComponentByClass(UMotionWarpingComponent::StaticClass()));

     //                    FVector OwnerWarpingLoc = Payload.Target->GetActorLocation() + Payload.Target->GetActorForwardVector() * CurHitReaction.ExecutionInfo->ExecutionWarpingDistance;
     //                    FRotator OwnerLookAtTargetRotation = UKismetMathLibrary::MakeRotFromX(-Payload.Target->GetActorForwardVector());

     //                    FRotator TargetLookAtOwnerRotation = UKismetMathLibrary::MakeRotFromX(-GetAvatarActorFromActorInfo()->GetActorForwardVector());
     //                    FVector TargetWarpingLoc = GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * CurHitReaction.ExecutionInfo->ExecutedWarpingDistance;

     //                    if (MotionWarpComp)
     //                    {
     //                        MotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Execution"), OwnerWarpingLoc, OwnerLookAtTargetRotation);
     //                    }

     //                    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Attack.CanNextAttack"));

     //                    if (MontgeTask)
     //                    {
     //                        MontgeTask->EndTask();
     //                    }
     //                    if (WaitInputTask)
     //                    {
     //                        WaitInputTask->EndTask();
     //                    }
     //                    if (MontageProxy)
     //                    {
     //                        MontageProxy->ReadyForActivation();
     //                    }

     //                    TargetHitReactionComp->ExecuteExecution(CurHitReaction.ExecutionInfo->ExecutedAnim, TargetWarpingLoc, TargetLookAtOwnerRotation);
     //                }
     //            }
     //        }
     //    }
     //}
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

    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Attack.CanNextAttack"));
    // 다음 콤보 인덱스 설정
    CurrentComboIndex = (CurrentComboIndex + 1) % ComboDataAsset->ComboInfos.Num();
    UE_LOG(LogTemp, Warning, TEXT("Next combo index: %d"), CurrentComboIndex);

    // 다음 애니메이션 실행
    Attack(CurrentComboIndex);
}

void UARPGMeleeAttackAbility::AttackParryEvent(FGameplayEventData Payload)
{
    UAbilityTask_PlayMontageAndWait* ParryMontgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ComboDataAsset->ComboInfos[CurrentActivateComboIndex].DeflectedMontage);
    ParryMontgeTask->ReadyForActivation();    
}

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


