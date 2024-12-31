// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActionFramework/ActionFramework.h"
#include "ARPGAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UARPGAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Ability")
	EARPGAbilityInputID InputID{ EARPGAbilityInputID::None };
};
