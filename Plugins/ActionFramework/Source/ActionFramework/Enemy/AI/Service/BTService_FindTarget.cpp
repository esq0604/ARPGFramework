// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Enemy/AI/Service/BTService_FindTarget.h"
#include "AIController.h"

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();


}
