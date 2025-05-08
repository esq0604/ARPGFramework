// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "ActionFramework/Interface/Combatable.h"
#include "ActionFramework/UI/OverlayPresenter.h"
#include "ARPGEnemy.generated.h"

class UTargetingComponent;
class UAbilitySystemComponent;
class UHitReactionComponent;
class UGameplayEffect;
class AWeaponItem;
class UBehaviorTree;
class UWidgetComponent;
class AARPGAIController;

//몬스터가 체력이 보여지는 조건
//1. 플레이어가 카메라 타겟으로 지정했을때
//2. 전투상태에 돌입했을 때.


UCLASS()
class ACTIONFRAMEWORK_API AARPGEnemy : public ACharacter , public ICombatable, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGEnemy();

	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual AActor* GetEquippedWeapon_Implementation() override;
	virtual void ToggleTargeting(bool bEnable) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }

	virtual void SetCombatTarget(AActor* NewCombatTarget) override;
	virtual AActor* GetCombatTarget() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintNativeEvent)
	void Dead();

	void Dead_Implementation();

	void InitDefaultAttribute();
private:
	void OnHealthChange(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount);

public:
	UPROPERTY()
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY()
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY()
	FOnAttributeChangedSignature OnPostureChanged;

	UPROPERTY()
	FOnAttributeChangedSignature OnMaxPostureChanged;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category = "ARPGEnemy")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ARPGEnemy")
	TObjectPtr<AWeaponItem> WeaponInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ARPGEnemy")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "ARPGEnemy") //플레이어가 감지하기 위한 컴포넌트 입니다
	TObjectPtr<UTargetingComponent> TargetingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ARPGEnemy") //플레이어가 타게팅시 보여질 ui를 위한 컴포넌트 입니다.
	TObjectPtr<UWidgetComponent> TargetWidgetComponent;
private:
	
	UPROPERTY(EditDefaultsOnly,Category="ARPGEnemy")
	TObjectPtr<UHitReactionComponent> HitReactionComponent;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AARPGAIController> ARPGAIController;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY()
	bool bHitReacting;

	UPROPERTY(EditDefaultsOnly)
	float BaseWalkSpeed;
};
