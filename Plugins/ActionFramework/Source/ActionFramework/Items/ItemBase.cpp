// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Items/ItemBase.h"
#include "ActionFramework/Inventory/InventoryComponent.h"

#include "Components/BoxComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	//SetRootComponent(StaticMeshComp);

	InteractCollision = CreateDefaultSubobject<UBoxComponent>("InteractCollision");
	InteractCollision->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AItemBase::BeginFocus()
{
}

void AItemBase::EndFocus()
{
}

void AItemBase::BeginInteraction()
{
}

void AItemBase::EndInteraction()
{
}

void AItemBase::Interact(AActor* Player)
{
	//IInteractable::Interact(PlayerCharacter);
	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(Player->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (InventoryComponent)
	{
		//InventoryComponent->AddItemToInventory(GetItemData());
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player have not Inventory Component"));
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

