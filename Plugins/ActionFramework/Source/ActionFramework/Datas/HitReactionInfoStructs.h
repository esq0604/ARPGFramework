// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HitReactionInfoStructs.generated.h"

/**
 * 
 */
class UGameplayEffect;
class UExecutionDataAsset;



USTRUCT(BlueprintType)
struct FHitReactionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    FGameplayTag AttackDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    FGameplayTag AttackType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    TSubclassOf<UGameplayEffect> DamageEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    float HitStop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    float Intensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    float CameraShake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction", meta = (EditCondition = "UseFinishEvent"))
    TObjectPtr<UExecutionDataAsset> ExecutionInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
    bool UseFinishEvent;
};
