// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "ARPGAnimInstance.h"

UARPGAnimInstance::UARPGAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UARPGAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		GameplayTagPropertyMap.Initialize(this, ASC);
	}
}
