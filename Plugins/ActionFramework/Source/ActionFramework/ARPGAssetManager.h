// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ARPGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UARPGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
	virtual void StartInitialLoading() override;
};
