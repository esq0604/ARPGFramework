// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecutePatternRow.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UBTTask_ExecutePatternRow : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PatternDataTableKeySelector;
		UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfActorKeySelector;

};
