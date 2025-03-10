// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGAbilitySystemGlobals.h"
#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
#include "ARPGAbilitySystemGlobals.h"

FGameplayEffectContext* UARPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FARPGGameplayEffectContext();
}
