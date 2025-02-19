// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ARPGPlayerController.generated.h"

/**
 * 
 */
class UARPGSpringArmComponent;
class UInputMappingContext;
class UARPGAbilitySystemComponent;
class UInputAction;
class UEscMenuWidget;
class UARPGInputConfig;
class AARPGPlayerState;
struct FInputActionValue;

UCLASS()
class ACTIONFRAMEWORK_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AARPGPlayerController(const FObjectInitializer& ObjectInitializer);

	AARPGPlayerState* GetARPGPlayerState() const;
	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const;
protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;


private:
	virtual void SetupInputComponent() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void TargetLock(const FInputActionValue& Value);

	void ToggleEscWidget(const FInputActionValue& Value);

	void ChangeNextWeapon(const FInputActionValue& Value);
	void ChangeNextTool(const FInputActionValue& Value);

	UFUNCTION()
	void HandleMenuButtonClicked(FGameplayTag ButtonTag);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
private:
	///** Jump Input Action */
	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* JumpAction{ nullptr };

	///** Move Input Action */
	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* MoveAction{ nullptr };

	///** Look Input Action */
	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* LookAction{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* TargetLockAction{ nullptr };
	//
	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* ToggleEscAction{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* LightAttackAction{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* BlockAction{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* NextWeaponAction{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Category = Input)
	//UInputAction* NextToolAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UARPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "ARPGController")
	TObjectPtr<UInputMappingContext> InputMapping;

	TObjectPtr<UARPGSpringArmComponent> SpringArmComponent;
	
	TObjectPtr<UARPGAbilitySystemComponent> ASC;
	bool bAnalogSettledSinceLastTargetSwitch = false;
	float TargetSwitchAnalogValue = .8f;
};
