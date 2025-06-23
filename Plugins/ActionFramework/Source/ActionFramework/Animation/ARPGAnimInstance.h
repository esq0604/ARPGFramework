// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
//#include "ActionFramework/Camera/ARPGSpringArmComponent.h"
#include "ARPGAnimInstance.generated.h"

class UAbilitySystemComponent;


/**
 * 
 */
UENUM(BlueprintType)
namespace EAnimation
{
	enum Direction : int
	{
		Foward,
		Backward,
		Left,
		Right
	};
}

USTRUCT(BlueprintType)
struct FAnimCadinalDirectionType
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Forward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Backward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right;
};


UCLASS()
class ACTIONFRAMEWORK_API UARPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UARPGAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);


private:
	UFUNCTION(BlueprintCallable ,meta = (BlueprintThreadSafe))
	void UpdateCharacterState();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateVelocityData();



	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UCharacterMovementComponent* GetMovementComponent();
public:
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly)
	FVector LookAtLocation;

	UPROPERTY(BlueprintReadWrite , Category = "CharacterState")
	bool bIsCrouch = false;	

	UPROPERTY(BlueprintReadWrite, Category = "CharacterState")
	bool bCrouchStateChange = false;
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	

};
