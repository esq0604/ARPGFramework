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
class UAbilitySystemComponent;
class UInputAction;
class UEscMenuWidget;
struct FInputActionValue;

UCLASS()
class ACTIONFRAMEWORK_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AARPGPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void TargetLock(const FInputActionValue& Value);

	void ToggleEscWidget(const FInputActionValue& Value);

	void LightAttack(const FInputActionValue& Value);

	void Block(const FInputActionValue& Value);

	UFUNCTION()
	void HandleMenuButtonClicked(FGameplayTag ButtonTag);

	UFUNCTION(BlueprintCallable)
	void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);
private:
	virtual void SetupInputComponent() override;



private:
	/** Jump Input Action */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* JumpAction{ nullptr };

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction{ nullptr };

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* TargetLockAction{ nullptr };
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ToggleEscAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LightAttackAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* BlockAction{ nullptr };


	UPROPERTY(EditDefaultsOnly, Category = "ARPGController")
	TSubclassOf<UUserWidget> EscMenuWidgetClass;
	TObjectPtr<UEscMenuWidget> EscMenuWidget;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "ARPGController")
	TObjectPtr<UInputMappingContext> InputMapping;

	TObjectPtr<UARPGSpringArmComponent> SpringArmComponent;
	
	TObjectPtr<UAbilitySystemComponent> ASC;
	bool bAnalogSettledSinceLastTargetSwitch = false;
	float TargetSwitchAnalogValue = .8f;
};
