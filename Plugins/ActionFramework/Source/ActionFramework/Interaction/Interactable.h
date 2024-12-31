// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class AActor;

UENUM()
enum class EInteractableType : uint8
{
	PickUp,
	NonPlayerCharacter,
	Device,
	Toggle,
	Container
};

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData(
		EInteractableType InInteractableType =EInteractableType::PickUp, 
		const FText& InName = FText::GetEmpty(),
		const FText& InAction = FText::GetEmpty(),
		int8 InQuantity =0,
		float InInterfaceDuration =0.f)
		:
		InteractableType(InInteractableType),
		Name(InName),
		Action(InAction),
		Quantity(InQuantity),
		InterfaceDuration(InInterfaceDuration)
	{

	};
	
	UPROPERTY(EditDefaultsOnly, Category="Interactable")
	EInteractableType InteractableType;

	UPROPERTY(EditDefaultsOnly, Category="Interactable")
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	FText Action;

	//픽업 아이템에만 사용됩니다.
	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	int8 Quantity;

	//문을 일정시간동안 여는 등 인터렉션에 타이머가 필요한 경우 사용됩니다.
	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	float InterfaceDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONFRAMEWORK_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus()=0;
	virtual void EndFocus()=0;
	virtual void BeginInteraction()=0;
	virtual void EndInteraction()=0;
	virtual void Interact(AActor* PlayerCharacter)=0;

	virtual FInteractableData* GetInteractableData();

};
