// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExecutionDataAsset.generated.h"

/**
 * 
 */
class UAnimMontage;

UCLASS()
class ACTIONFRAMEWORK_API UExecutionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="ExecutionInfo")
	TObjectPtr<UAnimMontage> ExecutionAnim;

	UPROPERTY(EditAnywhere, Category = "ExecutionInfo")
	TObjectPtr<UAnimMontage> ExecutedAnim;

	UPROPERTY(EditAnywhere, Category = "ExecutionInfo")
	float ExecutionWarpingDistance;

	UPROPERTY(EditAnywhere, Category = "ExecutionInfo")
	float ExecutedWarpingDistance;
};
