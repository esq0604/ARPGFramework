// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ModifyYawRate.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UANS_ModifyYawRate : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ModifyZYawRotateRate =1500.f;

private:
	UPROPERTY()
	FRotator DefaultRotateRate;

};
