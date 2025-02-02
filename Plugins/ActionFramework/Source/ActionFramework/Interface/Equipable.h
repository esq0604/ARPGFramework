// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equipable.generated.h"

// Equipable�̶�� ���� �κ��丮 ������Ʈ�� EquipmentWidget���� ������ ������ �����۵鿡 ���� ���� ���θ� �����ϴ� �������̽��Դϴ�.
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
