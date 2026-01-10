// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Animation/ARPGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "ARPGAnimInstance.h"

UARPGAnimInstance::UARPGAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UARPGAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	UpdateCharacterState();
	UpdateRotationData();
	UpdateVelocityData();
	UpdateLocationData(DeltaSeconds);
	UpdateAccelerationData();
	bIsFirstUpdate = false;
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
	const APawn* Owner = TryGetPawnOwner();
	if (Owner)
	{
		WorldVelocity = Owner->GetVelocity();

		FVector WorldVelocity2D = WorldVelocity * FVector(1, 1, 0);

		LocalVelocity2D = UKismetMathLibrary::LessLess_VectorRotator(WorldVelocity2D, WorldRotation);

		double Velocity = UKismetMathLibrary::VSizeXYSquared(LocalVelocity2D);
		
		LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);

		bHasVelocity = !UKismetMathLibrary::NearlyEqual_FloatFloat(Velocity, 0);
	}
}

void UARPGAnimInstance::UpdateRotationData()
{
	const AActor* Owner = GetOwningActor();
	if (Owner)
	{
		WorldRotation = Owner->GetActorRotation();
	}

}

void UARPGAnimInstance::UpdateLocationData(float DeltaTime)
{
	const AActor* Owner = GetOwningActor();
	if (Owner)
	{
		FVector OwnerLoc = Owner->GetActorLocation();

		DisplacementSinceLastUpdate =UKismetMathLibrary::VSizeXY(Owner->GetActorLocation() - WorldLocation);

		WorldLocation = Owner->GetActorLocation();

		DisplacementSpeed = UKismetMathLibrary::SafeDivide(DisplacementSinceLastUpdate, DeltaTime);

		if (bIsFirstUpdate)
		{
			DisplacementSinceLastUpdate = 0.f;
			DisplacementSpeed = 0.f;
		}
	}
}

void UARPGAnimInstance::UpdateAccelerationData()
{
	UCharacterMovementComponent* MovementComp = GetMovementComponent();

	if (MovementComp)
	{
		WorldAcceleration2D = MovementComp->GetCurrentAcceleration() * FVector(1.f, 1.f, 0.f);

		//캐릭터 좌표 가속도를 구하기 위해 UnRot , +-x, +-y를 통해 , wasd입력에 따른 가속도를 구함.
		LocalAcceleration2D =UKismetMathLibrary::LessLess_VectorRotator(WorldAcceleration2D, WorldRotation);

		//벡터길이 구하기 - 제곱근연산은 생략 , 제곱근 연산은 단순 비트연산으로 불가능함. 임계값 초과 여부만 보기위해 생략함
		//cpu는 제곱근을 위해 fpu 명령어 사용(sqtss, sqtpd) 이게 뉴턴랩슨 같은 반복근사 알고리즘을 하드웨어에 구현한것이라함.
		double Acceleration = UKismetMathLibrary::VSizeXYSquared(LocalAcceleration2D);

		if (AActor* Owner = GetOwningActor())
		{
			
			if (!WorldAcceleration2D.IsNearlyZero())
			{
				FVector Fwd = Owner->GetActorForwardVector();
				FVector Right = Owner->GetActorRightVector();

				float f = FVector::DotProduct(WorldAcceleration2D.GetSafeNormal2D(), Fwd);
				float r = FVector::DotProduct(WorldAcceleration2D.GetSafeNormal2D(), Right);

				if (FMath::Abs(f) >= FMath::Abs(r))
					MoveInputDirection = (f >= 0.f) ? EARPGAnimationDirection::Foward : EARPGAnimationDirection::Backward;
				else
					MoveInputDirection = (r >= 0.f) ? EARPGAnimationDirection::Right : EARPGAnimationDirection::Left;
			}

		}

		bHasAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(Acceleration, 0);

	}

}

EARPGAnimationDirection UARPGAnimInstance::SelectDirectionFromAngle(float Angle)
{
	// [-180, 180]로 정규화
	const float A = FRotator::NormalizeAxis(Angle);

	// 기준: +X를 0도로 보고, 시계 반대 방향으로 +각도
	// 구간: (-45,45] = Foward, (45,135] = Right, [-135,-45) = Left, 나머지 = Backward
	if (A > -45.f && A <= 45.f)
	{
		return EARPGAnimationDirection::Foward;   // 전진
	}
	if (A > 45.f && A <= 135.f)
	{
		return EARPGAnimationDirection::Right;    // 우측
	}
	if (A >= -135.f && A <= -45.f)
	{
		return EARPGAnimationDirection::Left;     // 좌측
	}
	return EARPGAnimationDirection::Backward;      // 후진 (135~180 또는 -180~-135)
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
