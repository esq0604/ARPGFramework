// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
#include "ActionFramework/AbilitySystem/ARPGAbilitySourceInterface.h"

ACTIONFRAMEWORK_API FARPGGameplayEffectContext* FARPGGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if (BaseEffectContext != nullptr)
	{
		if (UScriptStruct* Script = BaseEffectContext->GetScriptStruct())
		{
			UE_LOG(LogTemp, Warning, TEXT("Script valid, Struct Name: %s"), *Script->GetName());
			if (Script->IsChildOf(FARPGGameplayEffectContext::StaticStruct()))
			{

				UE_LOG(LogTemp, Warning, TEXT("is not child"));
				return (FARPGGameplayEffectContext*)BaseEffectContext;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Script  not vaild"));

		}
	}

	return nullptr;
}

void FARPGGameplayEffectContext::SetAbilitySource(const IARPGAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}

const IARPGAbilitySourceInterface* FARPGGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IARPGAbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FARPGGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
    return true;
}
