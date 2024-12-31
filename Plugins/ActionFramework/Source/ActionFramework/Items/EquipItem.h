// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/Items/ItemBase.h"
#include "ActionFramework/Interface/Useable.h"
#include "EquipItem.generated.h"

/**
 * 
 */
//EquipItem������ Equipable�� ������ �������� ���� �� �ǹ��մϴ�.
UCLASS()
class ACTIONFRAMEWORK_API AEquipItem : public AItemBase 
{
	GENERATED_BODY()
	
public:
	AEquipItem();
	virtual void Init(const UItemBaseDataAsset* InData);

	virtual void EquipMesh(const UItemBaseDataAsset* InData);
	virtual void EquipAbility(const UItemBaseDataAsset* InData);

	virtual void UnEquipMesh(const UItemBaseDataAsset* InData);
	virtual void UnEquipAbility();
	
private:
	UPROPERTY(EditDefaultsOnly,Category="EquipItem")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

};
