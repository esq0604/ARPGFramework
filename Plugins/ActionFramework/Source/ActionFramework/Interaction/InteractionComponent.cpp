// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "Interactable.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetWorld()->TimeSince(InteractionData.LastInteractCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void UInteractionComponent::Interact()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		if (IsInteracting())
		{
			TargetInteractable->EndInteraction();
			InteractionData.InteractState = false;
		}
		else
		{
			AActor* Owner = GetOwner();
			TargetInteractable->Interact(Owner);
			EInteractableType InteratableType = TargetInteractable->GetInteractableData()->InteractableType;
			switch (InteratableType)
			{
				case EInteractableType::PickUp:
				{
					UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(Owner->GetComponentByClass(UInventoryComponent::StaticClass()));
					
					break;
				}
				default:
				{
					break;
				}
			}

			InteractionData.InteractState = true;
		}
	}
}

void UInteractionComponent::PerformInteractionCheck()
{
	FVector TraceStart{ GetOwner()->GetActorLocation() };
	FVector TraceEnd{ TraceStart + (GetOwner()->GetActorForwardVector() * InteractionCheckDistance) };

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	TArray<FHitResult> TraceHits;
	if (GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(100.f), QueryParams))
	{
		for (const FHitResult& TraceHit : TraceHits)
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				//현재 인터렉팅인 대상이 아니라면 , 새로운대상을 찾은것.
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && TraceHit.GetActor() != nullptr)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				//현재 인터렉팅인 대상이라면 .
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	NoInteractableFound();
}

void UInteractionComponent::FoundInteractable(AActor* NewInteractable)
{
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	if (TargetInteractable == nullptr)
		return;


	if (TargetInteractable != nullptr)
		TargetInteractable->BeginFocus();
}

void UInteractionComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
	}

	InteractionData.CurrentInteractable = nullptr;
	TargetInteractable = nullptr;
}

void UInteractionComponent::BeginInteract()
{
	PerformInteractionCheck();
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteraction();
			Interact();

			/*if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InterfaceDuration, 0.1f))
			{
			}*/
			// else
			// {
			// 	GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
			// 		this,
			// 		&ACharacterBase::Interact,
			// 		TargetInteractable->InteractableData.InterfaceDuration,
			// 		false);
			// }
		}
	}
}

void UInteractionComponent::EndInteract()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteraction();
	}
}

