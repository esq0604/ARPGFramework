// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Items/EquipItem.h"
#include "ActionFramework/Datas/EquipBaseItemDataAsset.h"

AEquipItem::AEquipItem()
{

}

void AEquipItem::Init(const UItemBaseDataAsset* InData)
{
}

void AEquipItem::EquipMesh(const UItemBaseDataAsset* InData)
{

	UItemBaseDataAsset* nonConstItem = const_cast<UItemBaseDataAsset*>(InData);
	UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(nonConstItem);
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (MeshComp && EquipData)
	{
		for (const auto& MeshInfo : EquipData->ArmorData.MeshInfos)
		{
			USkeletalMeshComponent* NewMeshComp = NewObject<USkeletalMeshComponent>(this);
			NewMeshComp->SetSkinnedAssetAndUpdate(MeshInfo.SkeletalMesh);
			NewMeshComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, MeshInfo.AttachSocketName);
			NewMeshComp->RegisterComponent(); // 컴포넌트 등록
		}
	}
}

void AEquipItem::EquipAbility(const UItemBaseDataAsset* InData)
{

}

void AEquipItem::UnEquipMesh(const UItemBaseDataAsset* InData)
{
}

void AEquipItem::UnEquipAbility()
{
}
