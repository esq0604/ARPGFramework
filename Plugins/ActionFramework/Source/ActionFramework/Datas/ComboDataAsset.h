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
	TArray<FHitReactionInfo> HitReactionInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> EnemyHitVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ParryVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> WeaponHitVFX;
};


UCLASS()
class ACTIONFRAMEWORK_API UComboDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "ComboInfo")
	TArray<FComboInfo> ComboInfos;
};
