// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTService_SelectPatternRow.generated.h"

/**
 * 
 */
class UDataTable;
UCLASS()
class ACTIONFRAMEWORK_API UBTService_SelectPatternRow : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PatternDataTableKey;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UDataTable> PatternDataTable;
};
