// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGExecutedAbility.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UARPGExecutedAbility::UARPGExecutedAbility()
{
	FAbilityTriggerData TrrigerData;
	TrrigerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TrrigerData.TriggerTag = ARPGGameplayTags::GameplayEvent_Executed;
	AbilityTriggers.Add(TrrigerData);
	AbilityTags.AddTag(ARPGGameplayTags::Abilities_Executed);
}

void UARPGExecutedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->OptionalObject) return;

	const UExecutionDataAsset* ExecutionData = Cast<UExecutionDataAsset>(TriggerEventData->OptionalObject);
	if (!ExecutionData) return;


	UAbilityTask_PlayMontageAndWait* ExecuteAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), ExecutionData->ExecutedAnim);

	ExecuteAnimTask->ReadyForActivation();

	AActor* AvartarActor = GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvartarActor)
		return;

	UMotionWarpingComponent* MotionWarpingComp = AvartarActor->FindComponentByClass<UMotionWarpingComponent>();
	if (!MotionWarpingComp)
		return;

	UCapsuleComponent* Capsule = AvartarActor->FindComponentByClass<UCapsuleComponent>();
	if (Capsule)
	{

		Capsule->SetCollisionProfileName("EnemyDead"); 
	}
	// 공격자 위치
	FVector AttackerLoc = TriggerEventData->Target ? TriggerEventData->Target->GetActorLocation() : FVector::ZeroVector;
	FVector ExecutedLoc = AvartarActor->GetActorLocation();

	// 방향 벡터
	FVector ToAttacker = (AttackerLoc - ExecutedLoc).GetSafeNormal();
	FRotator LookAtRot = ToAttacker.Rotation();

	// 애니메이션 RootMotion 거리 계산
	FVector WarpLocation = ExecutedLoc + TriggerEventData->Target->GetActorForwardVector()* ExecutionData->ExecutedWarpingDistance; // 또는 AttackerLoc - Forward * 거리도 가능
	


	// 회전값을 워프 타겟으로 사용
	MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(WarpTargetName, WarpLocation, LookAtRot);
}

void UARPGExecutedAbility::MontageFinish()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);

}

void UARPGExecutedAbility::MontageCanceled()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}

void UARPGExecutedAbility::MontageInterrupted()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true);

}
