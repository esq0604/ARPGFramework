// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionFramework/Interaction/Interactable.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ItemBase.generated.h"

class UBoxComponent;
class UItemBaseDataAsset;

UCLASS()
class ACTIONFRAMEWORK_API AItemBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	

public:
	AItemBase();

#pragma region Interfaceable
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteraction() override;
	virtual void EndInteraction() override;
	virtual void Interact(AActor* Player);
#pragma endregion


	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item | Component", meta=(AllowPrivateAccess =" true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Item | Component")
	TObjectPtr<UBoxComponent> InteractCollision;
};
