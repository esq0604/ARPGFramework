// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

UENUM(BlueprintType)
enum class EARPGAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	Sprint			UMETA(DisplayName = "Sprint"),
	Jump			UMETA(DisplayName = "Junp"),
	LightAttack     UMETA(DisplayName = "LightAttack"),
	Block			UMETA(DisplayName = "Block")
};

class FActionFrameworkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
