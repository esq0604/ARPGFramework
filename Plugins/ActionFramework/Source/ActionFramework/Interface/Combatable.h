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
 * TODO : 인터페이스에서 GetEquippedWeapon은 Combatable과 관련이 없다고 생각이듬. 장착무기를 가져오는 로직은 InventoryComponent나, 다른곳에서 소유해야할거같다.
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
