// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "ARPGAnimInstance.generated.h"

class UAbilitySystemComponent;


/**
 * 
 */
UENUM(BlueprintType)
namespace EAnimation
{
	enum Direction : int
	{
		Foward,
		Backward,
		Left,
		Right
	};
}

USTRUCT(BlueprintType)
struct FForWayDirectionAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> Forward;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> Backward;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> Left;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> Right;
};


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
