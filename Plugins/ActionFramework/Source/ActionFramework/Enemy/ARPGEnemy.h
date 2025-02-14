// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "ActionFramework/Interface/Combatable.h"
#include "ARPGEnemy.generated.h"

class UTargetingComponent;
class UAbilitySystemComponent;
class UHitReactionComponent;
class UGameplayEffect;
class AWeaponItem;
class UBehaviorTree;
class AARPGAIController;

UCLASS()
class ACTIONFRAMEWORK_API AARPGEnemy : public ACharacter , public ICombatable, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGEnemy();

	virtual void PossessedBy(AController* NewController) override;

	virtual AActor* GetEquippedWeapon_Implementation() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintNativeEvent)
	void Dead();

	void Dead_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnHealthChange(const FOnAttributeChangeData& Data);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category = "ARPGEnemy")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ARPGEnemy")
	TObjectPtr<AWeaponItem> WeaponInstance;

private:
	UPROPERTY(EditDefaultsOnly,Category="ARPGEnemy")
	TObjectPtr<UTargetingComponent> TargetingComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="ARPGEnemy")
	TObjectPtr<UHitReactionComponent> HitReactionComponent;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AARPGAIController> ARPGAIController;

};
