// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ARPGAbility.generated.h"

/**
 * 
 */
class IARPGAbilitySourceInterface;
UCLASS()
class ACTIONFRAMEWORK_API UARPGAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StartupInputTag;


	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const IARPGAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;

};
