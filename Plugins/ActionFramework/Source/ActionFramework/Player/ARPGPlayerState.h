// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ARPGPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UARPGAbilitySystemComponent;
class UARPGAttributeSet;

UCLASS()
class ACTIONFRAMEWORK_API AARPGPlayerState : public APlayerState ,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AARPGPlayerState();

	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const { return ASC; }
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UARPGAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:



private:

	TObjectPtr<UARPGAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UARPGAbilitySystemComponent> ASC;
};
