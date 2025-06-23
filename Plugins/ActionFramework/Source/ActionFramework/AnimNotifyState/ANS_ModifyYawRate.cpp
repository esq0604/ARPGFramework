// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AnimNotifyState/ANS_ModifyYawRate.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ANS_ModifyYawRate.h"

void UANS_ModifyYawRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		if (UCharacterMovementComponent* Comp = Owner->GetComponentByClass<UCharacterMovementComponent>())
		{

			DefaultRotateRate = Comp->RotationRate;

			Comp->RotationRate = FRotator(DefaultRotateRate.Pitch, ModifyZYawRotateRate, DefaultRotateRate.Roll);
		}
	}
}

void UANS_ModifyYawRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		if (UCharacterMovementComponent* Comp = Owner->GetComponentByClass<UCharacterMovementComponent>())
		{
			Comp->RotationRate = DefaultRotateRate;
		}
	}
}
