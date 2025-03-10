// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGAbilitySystemComponent.h"
#include "ActionFramework/AbilitySystem/ARPGAbilitySystemGlobals.h"
#include "ActionFramework/AbilitySystem/ARPGAbility.h"

void UARPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilties)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilties)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
		if (const UARPGAbility* ARPGAbility = Cast<UARPGAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ARPGAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}
//
//FGameplayEffectContextHandle UARPGAbilitySystemComponent::MakeEffectContext() const
//{
//	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UARPGAbilitySystemGlobals::Get().AllocGameplayEffectContext());
//
//	// By default use the owner and avatar as the instigator and causer
//	if (ensureMsgf(AbilityActorInfo.IsValid(), TEXT("Unable to make effect context because AbilityActorInfo is not valid.")))
//	{
//		Context.AddInstigator(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get());
//	}
//
//	return Context;
//}

void UARPGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("AbilityInputPressByTag"));
			AbilitySpecInputPressed(AbilitySpec);
		}
	}
}

void UARPGAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UARPGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
		FScopedAbilityListLock ActiveScopeLoc(*this);
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
}

void UARPGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
