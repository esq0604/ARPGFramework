// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionFramework/Interface/Combatable.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "ARPGCharacter.generated.h"

class UHitReactionComponent;
class UInventoryComponent;
class UARPGSpringArmComponent;
struct FInputActionValue;
class UInputComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;

UCLASS()
class ACTIONFRAMEWORK_API AARPGCharacter : public ACharacter ,public IAbilitySystemInterface , public ICombatable, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AARPGCharacter();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:


protected:
	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;

	//virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void InitAbilityActorInfo();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class UARPGSpringArmComponent* GetSpringArmComp() const { return TargetingCameraSpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual AActor* GetEquippedWeapon_Implementation() override;
	virtual void ToggleTargeting(bool bEnable) override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

private:
	void AddCharacterAbilities();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ARPGCharacter")
	TObjectPtr<UARPGSpringArmComponent> TargetingCameraSpringArm;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ARPGCharacter")
	TObjectPtr<UCameraComponent> FollowCamera;
private:
	UPROPERTY(EditDefaultsOnly, Category = "ARPGCharacter")
	TObjectPtr<UHitReactionComponent> HitReactionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ARPGCharacter")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;
};
