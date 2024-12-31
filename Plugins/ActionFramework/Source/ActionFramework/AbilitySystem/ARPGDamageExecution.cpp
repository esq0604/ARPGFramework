// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"

struct FDamageStatics
{
	//Capturedef declarations for attributes.
	//속성에 대해 Capturedef를 정의합니다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	//디폴트 생성자입니다.
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, Damage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGAttributeSet, Health, Target, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UARPGDamageExecution::UARPGDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UARPGDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	//Obtain references to the target and source ABSC. Each copy of a GameplayAbilitySystem
		//타겟 및 소스에 대한 ABSC 참조를 얻습니다.
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	//변수와 태그를 사용하도록 게임플레이 이펙스펙을 가져옵니다.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//Aggregator Evaluate Parameters used during  the attribute capture.
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//Capturing the weapon Damage
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, BaseDamage);
	//BaseDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")),false,-1.0f),0.0f);
	//Performing the actual damage calculation

	//Final execution output. We can add more thane one AddOutputModifier to change multiple parameter at time base on complicated calculations. 
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -BaseDamage));

	//TargetABSC->GetAttributeSet(ARPGAttributeSet::)
}
