// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/GameNotifyCue_Hit.h"
#include "ActionFramework/Components/HitReactionComponent.h"

UGameNotifyCue_Hit::UGameNotifyCue_Hit()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Test"));
}

void UGameNotifyCue_Hit::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EGameplayCueEvent::Executed, Parameters);
}

bool UGameNotifyCue_Hit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Warning, TEXT("OnExecute In C++ Cue"));
	FVector Origin = Parameters.EffectContext.GetOrigin(); //Hit Impact 
	AActor* HitActor = Parameters.EffectContext.GetHitResult()->GetActor();

	FGameplayTag AttackDirection = SelectHitReaction(MyTarget, Origin);
	UActorComponent* FindComp = HitActor->GetComponentByClass(UHitReactionComponent::StaticClass());

	if (FindComp)
	{
		UHitReactionComponent* HitReactComp = Cast<UHitReactionComponent>(FindComp);
		HitReactComp->ExecuteHitReaction(AttackDirection);
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

FGameplayTag UGameNotifyCue_Hit::SelectHitReaction(AActor* TargetActor, FVector OriginPoint) const
{
	FVector TargetPos = TargetActor->GetActorLocation();

	// 1) ���� ���� (Ÿ�� ����)
	FVector Dir = OriginPoint - TargetPos;
	Dir.Z = 0.f;        // ���� ���
	Dir.Normalize();

	// 2) Ÿ���� ����, ���� ����
	FVector Forward = TargetActor->GetActorForwardVector();
	FVector Right = TargetActor->GetActorRightVector();
	Forward.Z = 0.f;   Forward.Normalize();
	Right.Z = 0.f;   Right.Normalize();

	// 3) ����
	float forwardDot = FVector::DotProduct(Forward, Dir); // ������� ����
	float rightDot = FVector::DotProduct(Right, Dir); // �������� ����
	// 4) ���� ��� (rad �� deg)

	// atan2(y, x)���� y = rightDot, x = forwardDot
	float angleDeg = FMath::RadiansToDegrees(FMath::Atan2(rightDot, forwardDot));
	// angleDeg ����: -180 ~ 180

	FString dirName;
	if (angleDeg >= -22.5f && angleDeg < 22.5f)
	{
		dirName = TEXT("F");
	}
	else if (angleDeg >= 22.5f && angleDeg < 67.5f)
	{
		dirName = TEXT("FR");
	}
	else if (angleDeg >= 67.5f && angleDeg < 112.5f)
	{
		dirName = TEXT("R");
	}
	else if (angleDeg >= 112.5f && angleDeg < 157.5f)
	{
		dirName = TEXT("BR");
	}
	else if (angleDeg >= 157.5f || angleDeg < -157.5f)
	{
		dirName = TEXT("B");
	}
	else if (angleDeg >= -157.5f && angleDeg < -112.5f)
	{
		dirName = TEXT("BL");
	}
	else if (angleDeg >= -112.5f && angleDeg < -67.5f)
	{
		dirName = TEXT("L");
	}
	else if (angleDeg >= -67.5f && angleDeg < -22.5f)
	{
		dirName = TEXT("FL");
	}

	// ���� dirName(F, FR, R, BR, B, BL, L, FL)�� ���
	UE_LOG(LogTemp, Warning, TEXT("8����: %s"), *dirName);
	return FGameplayTag();
}
