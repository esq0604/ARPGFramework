// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ARPGAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ACTIONFRAMEWORK_API UARPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;

	ATTRIBUTE_ACCESSORS(UARPGAttributeSet, Health)

		UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UARPGAttributeSet, MaxHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UARPGAttributeSet, Damage)

		UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UARPGAttributeSet, Stamina)

		UPROPERTY(BlueprintReadOnly, Category = "MaxStamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UARPGAttributeSet, MaxStamina)
	
private:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
