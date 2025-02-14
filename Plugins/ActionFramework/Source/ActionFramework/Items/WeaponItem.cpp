// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/Datas/EquipBaseItemDataAsset.h"
#include "ActionFramework/Components/WeaponCollisionComponent.h"
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

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponItem::OnWeaponOverlap);
}

void AWeaponItem::Init(const UItemBaseDataAsset* InData)
{
	Super::Init(InData);

	UItemBaseDataAsset* nonConstItem = const_cast<UItemBaseDataAsset*>(InData);
	UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(nonConstItem);

	//AActor* OwnerActor = GetOwner();

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
		FName WepaonStartSocketName = EquipData->WeaponData.WeaponSocketTraceName.TraceStartSocketName;
		FName WepaonEndSocketName = EquipData->WeaponData.WeaponSocketTraceName.TraceEndSocketName;

		FVector TraceStartLoc = GetStaticMeshComponent()->GetSocketTransform(WepaonStartSocketName, RTS_Component).GetLocation();
		FVector TraceEndLoc = GetStaticMeshComponent()->GetSocketTransform(WepaonEndSocketName, RTS_Component).GetLocation();

	
		float BoxExtendY = (abs(TraceStartLoc.Y) - abs(TraceEndLoc.Y)) / 2;
		float BoxCenterY = (TraceStartLoc.Y + TraceEndLoc.Y) / 2;

		FVector LocalBoundsMin, LocalBoundsMax;
		GetStaticMeshComponent()->GetLocalBounds(LocalBoundsMin, LocalBoundsMax);

		WeaponCollisionComponent->SetBoxExtent(FVector(LocalBoundsMax.X, BoxExtendY, LocalBoundsMax.Z));
		WeaponCollisionComponent->SetRelativeLocation(FVector(0.f, BoxCenterY, 0.f));
		WeaponCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}



}

void AWeaponItem::EquipAbility(const UItemBaseDataAsset* InData)
{

	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	UAbilitySystemComponent* ASC = nullptr;

	UItemBaseDataAsset* nonConstItem = const_cast<UItemBaseDataAsset*>(InData);
	UEquipBaseItemDataAsset* EquipData = Cast<UEquipBaseItemDataAsset>(nonConstItem);

	if (!ASI)
		return;
	if (!EquipData)
		return;

	ASC = ASI->GetAbilitySystemComponent();
	

	if (ASC)
	{
		for (const auto& Ability : EquipData->WeaponData.Abilties)
		{
			if (ensure(Ability))
			{
				UARPGAbility* AbilityCDO = Ability->GetDefaultObject<UARPGAbility>();

				FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1, static_cast<int32>(AbilityCDO->InputID), this);
				ASC->GiveAbility(AbilitySpec);
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC nullptr "));
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

void AWeaponItem::UnEquipAbility()
{
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

//장착된 무기의 좌표계 때문에 HitResult의 ImpactPoint를 수정하기위해 . const_cast를 사용했습니다.
void AWeaponItem::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AlreadyHitActor.Contains(SweepResult.GetActor()))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Weapon Overlap"));
	AlreadyHitActor.AddUnique(SweepResult.GetActor());
	IAbilitySystemInterface* IAS = Cast<IAbilitySystemInterface>(GetOwner());
	if (IAS)
	{
		FHitResult& HitResult = const_cast<FHitResult&>(SweepResult);
		HitResult.ImpactPoint = GetTransform().TransformPosition(SweepResult.Location);
		FGameplayEventData Payload;
		 
		Payload.Target = SweepResult.GetActor();
		Payload.ContextHandle = IAS->GetAbilitySystemComponent()->MakeEffectContext();
		
		Payload.ContextHandle.AddInstigator(GetOwner(), GetOwner());
		Payload.ContextHandle.AddHitResult(SweepResult);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), FGameplayTag::RequestGameplayTag("Attack.Hit"), Payload);
	}
}

void AWeaponItem::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


