// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "ARPGAnimInstance.generated.h"

class UAbilitySystemComponent;


/**
 * 
 */


UCLASS()
class ACTIONFRAMEWORK_API UARPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UARPGAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);


private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
