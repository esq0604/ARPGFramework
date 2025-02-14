// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARPGAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API AARPGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AARPGAIController();


private:

	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
