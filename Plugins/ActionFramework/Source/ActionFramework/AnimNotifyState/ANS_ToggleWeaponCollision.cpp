// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AnimNotifyState/ANS_ToggleWeaponCollision.h"
#include "ActionFramework/Interface/Combatable.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "Kismet/KismetSystemLibrary.h"

void UANS_ToggleWeaponCollision::NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			ICombatable* Combatable = Cast<ICombatable>(Owner);
			if (Combatable)
			{
				AActor* SpawnedActor = Combatable->Execute_GetEquippedWeapon(Owner);
				if (SpawnedActor)
				{
					AWeaponItem* Weapon = Cast<AWeaponItem>(SpawnedActor);
					if (Weapon)
					{
						Weapon->WeaponCollisionEnable(true);
					}
				}
			}
		}
	}
}


void UANS_ToggleWeaponCollision::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			ICombatable* Combatable = Cast<ICombatable>(Owner);
			if (Combatable)
			{
				AActor* SpawnedActor = Combatable->Execute_GetEquippedWeapon(Owner);
				if (SpawnedActor)
				{
					AWeaponItem* Weapon = Cast<AWeaponItem>(SpawnedActor);
					if (Weapon)
					{
						Weapon->WeaponCollisionEnable(false);
					}
				}
			}

		}
	}
}
