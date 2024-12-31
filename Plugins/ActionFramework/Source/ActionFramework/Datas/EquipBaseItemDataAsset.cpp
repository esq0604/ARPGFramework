// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Datas/EquipBaseItemDataAsset.h"
#include "ActionFramework/Items/EquipItem.h"

void UEquipBaseItemDataAsset::Use()
{
}

void UEquipBaseItemDataAsset::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("Equip In ItemDataAsset"));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = Cast<APawn>(GetOuter());
	const FTransform SpawnTransform = SpawnInfo.Owner->GetActorTransform();
	SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawnClass, SpawnTransform, SpawnInfo);

	AEquipItem* EquipItem = Cast<AEquipItem>(SpawnedActor);
	if (EquipItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawned Actor Type: %s"), *SpawnedActor->GetClass()->GetName());

		EquipItem->Init(this);
		EquipItem->EquipMesh(this);
		EquipItem->EquipAbility(this);
	}
}

void UEquipBaseItemDataAsset::UnEquip()
{

	AEquipItem* EquipItem = Cast<AEquipItem>(SpawnedActor);
	EquipItem->UnEquipAbility();
	EquipItem->UnEquipMesh(this);
	UE_LOG(LogTemp, Warning, TEXT("UnEquip Destoryed Actor Name : %s "),*SpawnedActor.GetName());
	if (SpawnedActor->Destroy())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnEquip Destory Succed Spawned Actor "));
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("UnEquip Destory Fail Spawned Actor "));
	}
}
