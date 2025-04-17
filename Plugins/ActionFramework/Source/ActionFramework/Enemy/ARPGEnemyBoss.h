// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/Enemy/ARPGEnemy.h"
#include "ARPGEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API AARPGEnemyBoss : public AARPGEnemy 
{
	GENERATED_BODY()
	
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//플레이어의 hud에 어떻게 체력이 보여질지.
	//1. 비헤이비어 트리 타겟감지 -> 상대 액터 접근 가능
	//2. 헬퍼함수 사용?

};
