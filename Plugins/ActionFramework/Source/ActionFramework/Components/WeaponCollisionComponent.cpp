// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Components/WeaponCollisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UWeaponCollisionComponent::UWeaponCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


//// Called when the game starts
//void UWeaponCollisionComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// ...
//	
//}
//
//// Called every frame
//void UWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	if (IsCollisionEnable)
//	{
//		TArray<FHitResult> HitResults;
//
//		FVector WeaponStartLoc = WeaponMeshComp->GetSocketLocation(WeaponStartName);
//		FVector WeaponEndLoc = WeaponMeshComp->GetSocketLocation(WeaponEndName);
//
//		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), WeaponStartLoc, WeaponEndLoc, 20.f, TraceObjectType, false, TraceActorToIgnore, EDrawDebugTrace::ForDuration, HitResults, true, FColor::Red, FColor::Green, 5.f);
//	}
//}


