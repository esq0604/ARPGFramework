// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGCueNotify_Parry.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ActionFramework/AbilitySystem/ARPGMeleeAttackAbility.h"
#include "ActionFramework/Components/HitReactionComponent.h"
#include "NiagaraFunctionLibrary.h"
UARPGCueNotify_Parry::UARPGCueNotify_Parry()
{
	GameplayCueTag = ARPGGameplayTags::GameplayCue_Parry;
}

bool UARPGCueNotify_Parry::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const UARPGMeleeAttackAbility* Ability = Cast<UARPGMeleeAttackAbility>(Parameters.EffectContext.GetAbility());
	const FComboInfo* ComboData = Ability->GetCurrentComboData();
	
	UNiagaraSystem* ParryNiagara = ComboData->ParryVFX;
	UNiagaraSystem* WeaponHitNiagara = ComboData->WeaponHitVFX;



	//��ü���� �ٸ� Reaction�� ����մϴ�.
	UHitReactionComponent* HitReactComp = Cast<UHitReactionComponent>(MyTarget->GetComponentByClass(UHitReactionComponent::StaticClass()));

	//ComboData�� �̿��ؼ� ���⿡ ���� �޶��������մϴ�.

	return false;
}
