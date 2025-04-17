// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Interface/Combatable.h"

// Add default functionality here for any ICombatable functions that are not pure virtual.

void ICombatable::ToggleTargeting(bool bEnable)
{
}

void ICombatable::ToggleHealth(bool bEnable)
{
}

void ICombatable::TogglePosture(bool bEnable)
{
}

void ICombatable::SetCombatTarget(AActor* NewCombatTarget)
{
}

AActor* ICombatable::GetCombatTarget()
{
	return nullptr;
}
