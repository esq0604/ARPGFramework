// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "HitReactionComponent.generated.h"

class UAnimMontage;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class ACTIONFRAMEWORK_API UHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitReactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ExecuteHitReaction(FGameplayTag HitDirectionTag);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ExecuteExecution(UAnimMontage* ExecutedMontage, FVector Location, FRotator Rotator);

	void ExecuteExecution_Implementation(UAnimMontage* ExecutedMontage, FVector Location, FRotator Rotator);

protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="HitReactionComp")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage> > HitReaction;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "HitReactionComp")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage> > ParryMontage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "HitReactionComp")
	TObjectPtr<UAnimMontage> BlockHitMontage;

private:

};
