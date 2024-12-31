// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractable;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractCheckTime(0.0f), InteractState(false)
	{
	};
	TObjectPtr<AActor> CurrentInteractable;

	UPROPERTY()
	float LastInteractCheckTime;

	UPROPERTY()
	bool InteractState;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONFRAMEWORK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE bool IsInteracting() const { return InteractionData.InteractState; }
	void Interact();
private:
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();

private:
	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY()
	TScriptInterface<IInteractable> TargetInteractable;


	float InteractionCheckFrequency{ 0.1f };
	float InteractionCheckDistance{ 225.0f };
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;
};
