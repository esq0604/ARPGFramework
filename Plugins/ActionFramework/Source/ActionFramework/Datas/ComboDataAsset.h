// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionFramework/Datas/HitReactionInfoStructs.h"
#include "ComboDataAsset.generated.h"

/**
 * 
 */

class UHitReactionData;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FComboInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComboInfo")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComboInfo")
	TObjectPtr<UAnimMontage> DeflectedMontage;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> EnemyHitVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ParryVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> WeaponHitVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution", meta = (EditCondition = "bUseExecuteEvent"))
	TObjectPtr<UExecutionDataAsset> ExecutionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	bool bUseExecuteEvent = false;
};


UCLASS()
class ACTIONFRAMEWORK_API UComboDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "ComboInfo")
	TArray<FComboInfo> ComboInfos;
};
