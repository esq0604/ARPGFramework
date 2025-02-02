// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equipable.generated.h"

// Equipable이라는 것은 인벤토리 컴포넌트의 EquipmentWidget에서 장착이 가능한 아이템들에 대해 대한 여부를 결정하는 인터페이스입니다.
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONFRAMEWORK_API IEquipable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool Equip() = 0;
	virtual bool UnEquip() = 0;
};
