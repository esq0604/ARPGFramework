// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/Datas/EquipBaseItemDataAsset.h"
#include "ActionFramework/Components/WeaponCollisionComponent.h"

#include "ActionFramework/ARPGGameplayTags.h"
#include "Components/BoxComponent.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AWeaponItem::AWeaponItem()
{
	WeaponCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionComp"));
	SetRootComponent(GetStaticMeshComponent());
	WeaponCollisionComponent->AttachToComponent(GetStaticMeshComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

AWeaponItem::AWeaponItem(int newtest) :
	test(newtest)
{
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponItem::OnWeaponOverlap);
}

void AWeaponItem::Init(const UItemBaseDataAsset* InData)
{
	Super::Init(InData);

	if (const UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(InData))
	{
		ComboData = EquipData->WeaponData.ComboDataAsset;
	}
}

void AWeaponItem::EquipMesh(const UItemBaseDataAsset* InData)
{
	Super::EquipMesh(InData);
	UItemBaseDataAsset* nonConstItem = const_cast<UItemBaseDataAsset*>(InData);
	UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(nonConstItem);
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (MeshComp && EquipData)
	{
		for (const auto& MeshInfo : EquipData->WeaponData.MeshInfos)
		{
			GetStaticMeshComponent()->SetStaticMesh(MeshInfo.StaticMesh);
			AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, MeshInfo.AttachSocketName);
			
		}
		MeshComp->LinkAnimClassLayers(EquipData->WeaponData.EquippedAnimSet);
	}

	if (EquipData && MeshComp)
	{
		const FVector TraceStartLoc = GetStaticMeshComponent()->GetSocketLocation(EquipData->WeaponData.WeaponSocketTraceName.TraceHeightStartSocketName);
		const FVector TraceEndLoc = GetStaticMeshComponent()->GetSocketLocation(EquipData->WeaponData.WeaponSocketTraceName.TraceHeightEndSocketName);

		const FVector TraceVector = TraceEndLoc - TraceStartLoc;
		const float TraceLength = TraceVector.Size();
		const FVector TraceDirection = TraceVector.GetSafeNormal();

		const FVector BoxCenter = (TraceStartLoc + TraceEndLoc) * 0.5f;

		WeaponCollisionComponent->SetWorldLocation(BoxCenter);

		// 박스 Extent 설정: 길이는 Trace 길이 / 2, 나머지는 임의값 또는 무기 크기 기반
		FVector BoxExtent;
		BoxExtent.X = TraceLength * 0.5f; // 길이
		BoxExtent.Y = 2.f; // 폭
		BoxExtent.Z = 2.f; // 높이
		WeaponCollisionComponent->SetBoxExtent(BoxExtent);

		// 박스 회전 설정 (Trace 방향에 맞게)
		FRotator BoxRotation = TraceDirection.Rotation();
		WeaponCollisionComponent->SetWorldRotation(BoxRotation);
	}



}


void AWeaponItem::UnEquipMesh(const UItemBaseDataAsset* InData)
{
	if (GetOwner())
	{
		USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

		UItemBaseDataAsset* nonConstItem = const_cast<UItemBaseDataAsset*>(InData);
		UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(nonConstItem);


		MeshComp->LinkAnimClassLayers(EquipData->WeaponData.UnEquippedAnimSet);
	}
}

void AWeaponItem::WeaponCollisionEnable(bool bEnable)
{
	if (bEnable)
	{
		WeaponCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AlreadyHitActor.Empty();
	}
}

void AWeaponItem::SetTraceObjectType(ECollisionChannel CollisionChannel, ECollisionResponse CollisionResponse)
{
	WeaponCollisionComponent->SetCollisionResponseToChannel(CollisionChannel, CollisionResponse);
}

void AWeaponItem::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AlreadyHitActor.Contains(SweepResult.GetActor()))
	{
		return;
	}
	AlreadyHitActor.AddUnique(SweepResult.GetActor());
	IAbilitySystemInterface* IAS = Cast<IAbilitySystemInterface>(GetOwner());
	if (IAS)
	{
		FHitResult& HitResult = const_cast<FHitResult&>(SweepResult);
		HitResult.ImpactPoint = GetTransform().TransformPosition(SweepResult.Location);
		FGameplayEventData Payload;
	
		Payload.Target = SweepResult.GetActor();
		Payload.ContextHandle = IAS->GetAbilitySystemComponent()->MakeEffectContext();
		Payload.ContextHandle.AddSourceObject(ComboData);
		Payload.ContextHandle.AddInstigator(GetOwner(), GetOwner());
		Payload.ContextHandle.AddHitResult(SweepResult);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(),ARPGGameplayTags::GameplayEvent_Attack_Hit, Payload);
	}
}

void AWeaponItem::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


