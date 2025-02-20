// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/Items/EquipItem.h"
#include "GameplayTagContainer.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
class UWeaponCollisionComponent;
class UItemBaseDataAsset;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class ACTIONFRAMEWORK_API AWeaponItem : public AEquipItem
{
	GENERATED_BODY()
	
public:
	AWeaponItem();

	virtual void BeginPlay() override;

	virtual void Init(const UItemBaseDataAsset* InData) override;

	UFUNCTION(BlueprintCallable)
	virtual void EquipMesh(const UItemBaseDataAsset* InData) override;
	virtual void EquipAbility(const UItemBaseDataAsset* InData) override;
		
	UFUNCTION(BlueprintCallable)
	virtual void UnEquipMesh(const UItemBaseDataAsset* InData) override;
	virtual void UnEquipAbility() override;

	UFUNCTION(BlueprintCallable)
	void WeaponCollisionEnable(bool bEnable);

	UFUNCTION(BlueprintCallable)
	void SetTraceObjectType(ECollisionChannel CollisionChannel, ECollisionResponse CollisionResponse);

private:
	UFUNCTION(BlueprintCallable)
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//void 

protected:

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Item | Component")
	TObjectPtr<UBoxComponent> WeaponCollisionComponent;
private:
	TObjectPtr<class UComboDataAsset> ComboData;

	TArray<TObjectPtr<AActor>> AlreadyHitActor;
};
