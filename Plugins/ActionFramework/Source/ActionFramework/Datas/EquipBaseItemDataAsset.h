// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ActionFramework/Interface/Useable.h"
#include "EquipBaseItemDataAsset.generated.h"

/**
 * 
 */

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "EquipData")
	TArray<TSubclassOf<UARPGAbility>> Abilties;

	UPROPERTY(EditAnywhere,Category = "EquipData")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditAnywhere,Category = "EquipData")
	TArray<FMeshInfos> MeshInfos;

	UPROPERTY(EditAnywhere,Category = "EquipData")
	FWeaponTraceSocketName WeaponSocketTraceName;

	UPROPERTY(EditAnywhere, Category = "EquipData")
	TSubclassOf<UAnimInstance> EquippedAnimSet;

	UPROPERTY(EditAnywhere, Category = "EquipData")
	TSubclassOf<UAnimInstance> UnEquippedAnimSet;
};

USTRUCT()
struct FArmorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "EquipData")
	FAbilitySet_GameplayEffect Effect;

	UPROPERTY(EditAnywhere, Category = "EquipData")
	TArray<FMeshInfos> MeshInfos;
};


//ItemData에서 Equip, Use는 아이템을 EquipWidget에 장착 및 사용을 의미합니다.
UCLASS()
class ACTIONFRAMEWORK_API UEquipBaseItemDataAsset : public UItemBaseDataAsset , public IEquipable , public IUseable
{
	GENERATED_BODY()
\
public:
	virtual void Use() override;

	virtual void Equip() override;
	virtual void UnEquip() override;

public:
	UPROPERTY(EditAnywhere, Category = "EquipData", meta = (EditCondition = "bIsWeapon"))
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere, Category = "EquipData", meta = (EditCondition = "bArmor"))
	FArmorData ArmorData;
	// 플래그
	UPROPERTY(EditAnywhere, Category = "ItemFlags")
	bool bIsWeapon = false;

	UPROPERTY(EditAnywhere, Category = "ItemFlags")
	bool bArmor = false;
};
