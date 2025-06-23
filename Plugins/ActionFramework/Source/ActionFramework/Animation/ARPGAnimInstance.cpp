// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARPGAnimInstance.h"

UARPGAnimInstance::UARPGAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UARPGAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	UpdateCharacterState();

}

void UARPGAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		GameplayTagPropertyMap.Initialize(this, ASC);
	}
}

void UARPGAnimInstance::UpdateCharacterState()
{
	bool bWasCrouchLastUpdate = bIsCrouch;

	UCharacterMovementComponent* MovementComp = GetMovementComponent();
	if (!MovementComp)
		return;
	
	bIsCrouch = MovementComp->IsCrouching();	

	if (bWasCrouchLastUpdate != bIsCrouch)
	{
		bCrouchStateChange = true;
	}
	else
	{
		bCrouchStateChange = false;
	}


}

void UARPGAnimInstance::UpdateVelocityData()
{
}

UCharacterMovementComponent* UARPGAnimInstance::GetMovementComponent()
{
	APawn* Owner = TryGetPawnOwner();
	if (!Owner)
		return nullptr;
	UPawnMovementComponent* MovementComp = Owner->GetMovementComponent();
	if (!MovementComp)
		return nullptr;

	UCharacterMovementComponent* CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
	if (!CharacterMoveComp)
		return nullptr;

	return CharacterMoveComp;
}
