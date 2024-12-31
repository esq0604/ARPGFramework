// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combatable.generated.h"

// This class does not need to be modified.
class AActor;

UINTERFACE(MinimalAPI,BlueprintType)
class UCombatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * TODO : �������̽����� GetEquippedWeapon�� Combatable�� ������ ���ٰ� �����̵�. �������⸦ �������� ������ InventoryComponent��, �ٸ������� �����ؾ��ҰŰ���.
 */
class ACTIONFRAMEWORK_API ICombatable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* GetEquippedWeapon();
	virtual AActor* GetEquippedWeapon_Implementation() = 0;
	virtual void ToggleTargeting(bool bEnable);
};
