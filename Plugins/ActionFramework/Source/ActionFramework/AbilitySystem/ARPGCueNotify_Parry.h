// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARPGCueNotify_Parry.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UARPGCueNotify_Parry : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UARPGCueNotify_Parry();

	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

};
