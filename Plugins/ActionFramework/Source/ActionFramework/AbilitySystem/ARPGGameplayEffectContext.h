// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ARPGGameplayEffectContext.generated.h"

/**
 * 
 */
USTRUCT()
struct FARPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	FARPGGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FARPGGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

protected:
	struct FComboInfo ComboInfo;



};
