// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
// Sets default values for this component's properties
UHitReactionComponent::UHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitReactionComponent::PlayParryMontage(UAnimMontage* ExecutedMontage)
{
	ACharacter* OwnerCharaacter = Cast<ACharacter>(GetOwner());
	OwnerCharaacter->PlayAnimMontage(ExecutedMontage);
}

void UHitReactionComponent::ExecuteHitReaction(FGameplayTag HitDirectionTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Tag Name : %s"), *HitDirectionTag.ToString());
	if (GetOwner() != nullptr)
	{
		ACharacter* OwnerCharaacter = Cast<ACharacter>(GetOwner());
		if(HitReaction.Find(HitDirectionTag) !=nullptr)
		{
			UAnimMontage* FindMontage = *HitReaction.Find(HitDirectionTag);
			if (FindMontage)
			{
				float Duration = OwnerCharaacter->PlayAnimMontage(FindMontage);
				if (Duration == 0.f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Play Not HitReaction Montage"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Play  HitReaction Montage"));

				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Tag Anim null"));
		}

	}
}

//void UHitReactionComponent::ExecuteExecution(UAnimMontage* ExecutedMontage , FVector Location,FRotator Rotator)
//{
//
//}

void UHitReactionComponent::ExecuteExecution_Implementation(UAnimMontage* ExecutedMontage, FVector Location, FRotator Rotator)
{
	if (GetOwner() != nullptr)
	{
		ACharacter* OwnerCharaacter = Cast<ACharacter>(GetOwner());
		UMotionWarpingComponent* MotionWarpingComp = Cast<UMotionWarpingComponent>(GetOwner()->GetComponentByClass(UMotionWarpingComponent::StaticClass()));
		if (ExecutedMontage && MotionWarpingComp)
		{
			
			MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Executed"), Location, Rotator);
			UE_LOG(LogTemp, Warning, TEXT("PlayExecutionMontage"));
			OwnerCharaacter->PlayAnimMontage(ExecutedMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Tag Anim null"));
		}

	}
}

