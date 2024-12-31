// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Useable.generated.h"

// Useable이라는 인터페이스는 아이템을 사용할수있냐는 인터페이스입니다. 즉 장비아이템을 사용하면 장착을하고 , 소비아이템을 사용하면 효과가 발동된다입니다.
// 
// This class does not need to be modified.

class UItemBaseDataAsset;

UINTERFACE(MinimalAPI)
class UUseable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONFRAMEWORK_API IUseable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Use()=0;
};
