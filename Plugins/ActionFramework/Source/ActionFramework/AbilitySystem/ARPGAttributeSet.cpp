// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "ActionFramework/AbilitySystem/ARPGGameplayEffectContext.h"
#include "ActionFramework/Datas/ExecutionDataAsset.h"
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
	if (Attribute == GetPostureAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxPosture());
	}
}

bool UARPGAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	bool bResult = Super::PreGameplayEffectExecute(Data);
	//Data.EffectSpec
	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);
	if (Properties.SourceCharacter && Properties.TargetCharacter)
	{
		FVector SourceForward = Properties.SourceCharacter->GetActorForwardVector().GetSafeNormal();
		FVector ToTarget = (Properties.TargetCharacter->GetActorLocation() - Properties.SourceCharacter->GetActorLocation()).GetSafeNormal();
		float DotProduct = FVector::DotProduct(SourceForward, ToTarget);

		//정면 및 Parry상태라면
		if (Properties.TargetASC->HasMatchingGameplayTag(ARPGGameplayTags::Status_Parry) && DotProduct > 0.f)
		{
			FGameplayEventData Payload;
			Payload.Instigator = Properties.SourceCharacter;
			Payload.Target = Properties.TargetCharacter;
			Payload.ContextHandle = Properties.EffectContextHandle;
			Properties.TargetASC->HandleGameplayEvent(ARPGGameplayTags::GameplayEvent_Parry, &Payload);

			return bResult;
		}
		//정면 및 방어상태라면
		else if (Properties.TargetASC->HasMatchingGameplayTag(ARPGGameplayTags::Status_Block) && DotProduct > 0.f)
		{
			FGameplayEventData Payload;
			Payload.ContextHandle = Properties.EffectContextHandle;
			Properties.TargetASC->HandleGameplayEvent(ARPGGameplayTags::GameplayEvent_Block, &Payload);

			return bResult = true;
		}
	}

	return bResult = true;
}

//게임플레이 이펙트가 적용된 후 실행됩니다. 여기서 GA_HitReact, DamageFloating 등을 수행하도록합니다.
//그럼 기존의 GA_Attack에서 적의 정보를 포함하고, 적의 HitReact를 실행하는것보다. 다른 클래스와의 의존도가 떨어짐.
void UARPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (Properties.TargetCharacter && Properties.SourceCharacter)
		{
			HandleHitReact(Properties);
			if (GetHealth() <= 0.f)
			{
				TryTrrigerExecute(Properties);
			}
		}
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
	if (Attribute == GetMaxPostureAttribute() && bTopOffPosture)
	{
		SetPosture(GetMaxPosture());
		bTopOffPosture = false;
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

void UARPGAttributeSet::HandleHitReact(const FEffectProperties& Props)
{
	FGameplayEventData Payload;
	Payload.Instigator = Props.SourceCharacter;
	Payload.Target = Props.TargetCharacter;
	Payload.ContextHandle = Props.EffectContextHandle;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Handle GameplayEvent Hit React"));
	Props.TargetASC->HandleGameplayEvent(ARPGGameplayTags::GameplayEvent_HitReact, &Payload);
}

void UARPGAttributeSet::TryTrrigerExecute(const FEffectProperties& Props)
{
	const FARPGGameplayEffectContext* ARPGContext = static_cast<const FARPGGameplayEffectContext*>(Props.EffectContextHandle.Get());
	if (!ARPGContext) return;

	UE_LOG(LogTemp, Warning, TEXT("%s , TryExecute GetName : %s"), *FString(__FILE__), *Props.TargetCharacter->GetName());

	const int32 ComboIndex = ARPGContext->ComboIndex;
	const int32 HitReactIndex = ARPGContext->HitReactIndex;

	//공격자 처형 어빌리티 수행,//피격자 처형 어빌리티 수행
	if (const UComboDataAsset* ComboDataAsset = Cast<UComboDataAsset>(ARPGContext->GetSourceObject()))
	{
		if (!ComboDataAsset->ComboInfos.IsValidIndex(ComboIndex))
		{
			return;
		}

		if (!ComboDataAsset->ComboInfos[ComboIndex].bUseExecuteEvent) return;

		FGameplayEventData ExecutionEvent;
		ExecutionEvent.EventTag = ARPGGameplayTags::GameplayEvent_Execution;
		ExecutionEvent.Instigator = Props.TargetCharacter;  // 피해자 → 이벤트 발생
		ExecutionEvent.Target = Props.SourceCharacter;      // 공격자 → 실행 주체
		ExecutionEvent.ContextHandle = Props.EffectContextHandle;
		ExecutionEvent.OptionalObject = Cast<const UObject>(ComboDataAsset->ComboInfos[ComboIndex].ExecutionData.Get());

		UE_LOG(LogTemp, Warning, TEXT("ExecutionEvent SourceCharacter : %s"), *Props.SourceCharacter->GetName());
		UE_LOG(LogTemp, Warning, TEXT("ExecutionEvent TargetCharacter : %s"), *Props.TargetCharacter->GetName());

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Props.SourceCharacter,
			ARPGGameplayTags::GameplayEvent_Execution,
			ExecutionEvent
		);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Props.TargetCharacter,
			ARPGGameplayTags::GameplayEvent_Executed,
			ExecutionEvent
		);
		
	}


}