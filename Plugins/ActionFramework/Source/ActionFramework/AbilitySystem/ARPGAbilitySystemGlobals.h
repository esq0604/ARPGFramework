// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ARPGAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class ACTIONFRAMEWORK_API UARPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
