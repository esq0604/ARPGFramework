// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"
#include "ActionFramework/Interface/Combatable.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{

}

void UTargetingComponent::HandleTargetingStateChanged(bool bIsTargeted)
{
	if (AActor* Owner = GetOwner())
	{
		if (ICombatable* Combatable = Cast<ICombatable>(Owner))
		{
			Combatable->ToggleTargeting(bIsTargeted);
		}
	}
}


