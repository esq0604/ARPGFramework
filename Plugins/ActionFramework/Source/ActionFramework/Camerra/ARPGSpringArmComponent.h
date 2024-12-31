// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ARPGSpringArmComponent.generated.h"

UENUM(BlueprintType)
enum class ECameraDirection : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};
class UTargetingComponent;
class UCharacterMovementComponent;
class UCurveFloat;
/**
 * 
 */
UCLASS(Blueprintable ,ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONFRAMEWORK_API UARPGSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
public:
	UARPGSpringArmComponent();

	void ToggleCameraLock();
	void ToggleSoftLock();
	bool IsCameraLockedToTarget();
	void SwitchTarget(ECameraDirection SwitchDirection);
	
private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LockToTarget(UTargetingComponent* NewTargetComponent);
	void BreakTargetLock();

	void AdjustCameraBasedOnTargetDistance(UTargetingComponent* NewTargetComponent);

	class UTargetingComponent* GetLockTarget();
	TArray<UTargetingComponent*> GetTargetComponents();

public:
	UPROPERTY(BlueprintReadOnly)
	UTargetingComponent* CameraTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float LockonControlRotationRate = 10.f;
private:
	/* ���Ͱ� Ÿ���� �� �� �ִ� ĳ���Ϳ��� �ִ� �Ÿ� */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float MaxTargetLockDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool bUseSoftLock;

	/* Turn debug visuals on/off */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool bDrawDebug{ true };

	/* True if lock was recently broken and mouse delta is still high */
	bool bSoftlockRequiresReset;

	//�Ϲ����� ��Ȳ���� ī�޶� �÷��̾��� �������� ������� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float CommonCameraLagSpeed{ 5.f };

	//Ÿ���� ��Ȳ���� ī�޶� �÷��̾��� �������� ������¼ӵ� (Ÿ���û�Ȳ���� ������ �ֹ̳��ϴ�)
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float TargetCameraLagSpeed{ 10.f };
	/* The component the camera is currently locked on to */

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	TObjectPtr<UCurveFloat> SpringArmZOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	TObjectPtr<UCurveFloat> SpringArmLenght;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	TObjectPtr<UCurveFloat> CameraRotation;


	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float DefaultSpringArmLenght{ 400.f };

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float DefaultSpringArmZOffSet{ 0.f };

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	FRotator DefaultCameraRotation{ 0.f,0.f,0.f };
	
};
