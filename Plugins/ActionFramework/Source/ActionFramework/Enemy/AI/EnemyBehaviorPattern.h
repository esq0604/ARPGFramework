// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "Engine/DataTable.h"
#include "EnemyBehaviorPattern.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnemyBehaviorPattern : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EnemyTag; //몬스터 구분

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag; //사용 어빌리티 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPercentThreshold; //HealthPercentThreshold 이하 일때 사용

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Once;	//한번만 사용
};