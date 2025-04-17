// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BossManager.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UBossManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void SetBoss(APawn* NewBoss);
};
