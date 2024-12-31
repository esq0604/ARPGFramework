// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ItemBaseDataAsset.generated.h"


/**
 *
 */

class AItemBase;
class UGameplayAbility;
class UGameplayEffect;

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="ItemData")
	FText Name;

	UPROPERTY(EditAnywhere,Category = "ItemData")
	FText Description;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FText InteractionText;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FText UsingText;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	UTexture2D* Icon{ nullptr };

	UPROPERTY(EditAnywhere, Category = "ItemData")
	UStaticMesh* DropMesh{ nullptr };
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	float SellValue{ 0 };
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	int32 Quantity{ 0 };

	UPROPERTY(EditAnywhere, Category = "ItemData")
	float Weight{ 0 };

	UPROPERTY(EditAnywhere, Category = "ItemData")
	int32 MaxStackSize{ 0 };

	UPROPERTY(EditAnywhere, Category = "ItemData")
	bool IsStackable{ false };
};


USTRUCT()
struct FWeaponTraceSocketName
{
	GENERATED_BODY()

	FWeaponTraceSocketName() :
		TraceStartSocketName(""),
		TraceEndSocketName("")
	{
	}

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName TraceStartSocketName;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName TraceEndSocketName;
};

USTRUCT()
struct FMeshInfos
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName AttachSocketName;
};

USTRUCT()
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	// Gameplay ability to grant.
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TSubclassOf<UGameplayAbility> Ability{ nullptr };
};

USTRUCT()
struct FAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	// Gameplay effect to grant.
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TSubclassOf<UGameplayEffect> GameplayEffect{ nullptr };
};




UCLASS()
class ACTIONFRAMEWORK_API UItemBaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	AActor* GetSpawnedActor();

public:
	UPROPERTY(EditAnywhere, Category = "ItemData")
	FText ID;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FGameplayTag ItemTypeTag;


	UPROPERTY(EditAnywhere, Category = "ItemData")
	TSubclassOf<AActor> ActorToSpawnClass;

	TObjectPtr<AActor> SpawnedActor;
};
