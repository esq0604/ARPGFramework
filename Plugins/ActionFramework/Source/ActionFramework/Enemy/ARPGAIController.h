// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARPGAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API AARPGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AARPGAIController();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
};
