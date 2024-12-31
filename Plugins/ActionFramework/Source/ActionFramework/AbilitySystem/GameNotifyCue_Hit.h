// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayTagContainer.h"
#include "GameNotifyCue_Hit.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, Category = "GameplayCueNotify")
class ACTIONFRAMEWORK_API UGameNotifyCue_Hit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGameNotifyCue_Hit();

	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:
	 bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	FGameplayTag SelectHitReaction(AActor* TargetActor, FVector OriginPoint) const;

};
