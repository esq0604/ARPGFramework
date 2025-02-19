// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"


UARPGAttributeSet::UARPGAttributeSet()
{
}

void UARPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

bool UARPGAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	bool bResult = Super::PreGameplayEffectExecute(Data);
	//Data.EffectSpec
	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);
	FVector SourceForward = Properties.SourceCharacter->GetActorForwardVector().GetSafeNormal();
	FVector ToTarget = (Properties.TargetCharacter->GetActorLocation() - Properties.SourceCharacter->GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(SourceForward, ToTarget);

	if (Properties.TargetASC->HasMatchingGameplayTag(ARPGGameplayTags::Status_Parry) && DotProduct>0.f)
	{
		FGameplayCueParameters CueParams;
		CueParams.EffectContext = Properties.EffectContextHandle;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("HasMatching Tag Parry"));
		Properties.TargetASC->ExecuteGameplayCue(ARPGGameplayTags::GameplayCue_Parry, CueParams);
	}
	return bResult;
}

//게임플레이 이펙트가 적용된 후 실행됩니다. 여기서 GA_HitReact, DamageFloating 등을 수행하도록합니다.
//그럼 기존의 GA_Attack에서 적의 정보를 포함하고, 적의 HitReact를 실행하는것보다. 다른 클래스와의 의존도가 떨어짐.
void UARPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (const UGameplayAbility* SourceAbility = Data.EffectSpec.GetContext().GetAbility())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,
			FString::Printf(TEXT("PostGameplayEffectExecute, GetContextAbility: %s"), *SourceAbility->GetName()));
	}

}

void UARPGAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxStaminaAttribute() && bTopOffStamina)
	{
		SetStamina(GetMaxStamina());
		bTopOffStamina = false;
	}
}

void UARPGAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

