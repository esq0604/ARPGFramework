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
enum class EARPGAnimationDirection : uint8
{
	Foward,
	Backward,
	Left,
	Right
};


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
	void UpdateRotationData();

	UFUNCTION(BlueprintCallable, meta =(BlueprintThreadSafe))
	void UpdateLocationData(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateAccelerationData();

	UFUNCTION()
	EARPGAnimationDirection SelectDirectionFromAngle(float Angle);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UCharacterMovementComponent* GetMovementComponent();
public:
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly)
	FVector LookAtLocation;

	UPROPERTY(BlueprintReadWrite , Category = "CharacterState")
	bool bIsCrouch = false;	

	UPROPERTY(BlueprintReadWrite, Category = "CharacterState")
	bool bCrouchStateChange = false;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector WorldVelocity;

	UPROPERTY(BlueprintReadOnly)
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadOnly)
	FVector WorldLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector LocalVelocity2D;

	UPROPERTY(BlueprintReadOnly)
	FVector WorldAcceleration2D;

	UPROPERTY(BlueprintReadOnly)
	FVector LocalAcceleration2D;

	UPROPERTY(BlueprintReadOnly)
	bool bHasVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool bHasAcceleration;

	UPROPERTY(BlueprintReadOnly)
	float DisplacementSpeed;

	UPROPERTY(BlueprintReadOnly)
	float DisplacementSinceLastUpdate;

	UPROPERTY(BlueprintReadOnly)
	float LocalVelocityDirectionAngle;

	UPROPERTY(BlueprintReadOnly)
	float WorldVelocityDirectionAngle;

	UPROPERTY(BlueprintReadOnly)
	EARPGAnimationDirection MoveInputDirection;

	bool bIsFirstUpdate=true;
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;


};
