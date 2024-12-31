// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AnimNotifyState/ANS_AddRemoveGameplayTag.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "ANS_AddRemoveGameplayTag.h"

void UANS_AddRemoveGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();

	IAbilitySystemInterface* ASCI = Cast<IAbilitySystemInterface>(Owner);

	if (ASCI)
	{
		UAbilitySystemComponent* ASC = ASCI->GetAbilitySystemComponent();

		if (ASC)
		{
			ASC->AddLooseGameplayTag(Tag);
		}
	}
}

void UANS_AddRemoveGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();

	IAbilitySystemInterface* ASCI = Cast<IAbilitySystemInterface>(Owner);

	if (ASCI)
	{
		UAbilitySystemComponent* ASC = ASCI->GetAbilitySystemComponent();

		if (ASC)
		{
			//ASC->RemoveLooseGameplayTag(Tag);
		}
	}
}
