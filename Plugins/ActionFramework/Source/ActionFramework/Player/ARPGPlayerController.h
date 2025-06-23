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

	void Crouch(const FInputActionValue& Value);
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

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UARPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "ARPGController")
	TObjectPtr<UInputMappingContext> InputMapping;

	TObjectPtr<UARPGSpringArmComponent> SpringArmComponent;
	
	TObjectPtr<UARPGAbilitySystemComponent> ASC;
	bool bAnalogSettledSinceLastTargetSwitch = false;
	float TargetSwitchAnalogValue = .8f;
};
