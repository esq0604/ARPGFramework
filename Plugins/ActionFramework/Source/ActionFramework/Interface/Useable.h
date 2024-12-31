// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Useable.generated.h"

// Useable�̶�� �������̽��� �������� ����Ҽ��ֳĴ� �������̽��Դϴ�. �� ���������� ����ϸ� �������ϰ� , �Һ�������� ����ϸ� ȿ���� �ߵ��ȴ��Դϴ�.
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
