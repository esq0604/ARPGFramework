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



	//객체마다 다른 Reaction을 사용합니다.
	UHitReactionComponent* HitReactComp = Cast<UHitReactionComponent>(MyTarget->GetComponentByClass(UHitReactionComponent::StaticClass()));

	//ComboData를 이용해서 무기에 따라 달라지도록합니다.

	return false;
}
