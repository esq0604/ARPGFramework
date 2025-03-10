// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "ActionFramework/Datas/ComboDataAsset.h"
#include "ARPGGameplayEffectContext.generated.h"

/**
 * 
 */
class IARPGAbilitySourceInterface;
USTRUCT()
struct FARPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	FARPGGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FARPGGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	static ACTIONFRAMEWORK_API FARPGGameplayEffectContext*  ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const IARPGAbilitySourceInterface* InObject, float InSourceLevel);
	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const IARPGAbilitySourceInterface* GetAbilitySource() const;
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FARPGGameplayEffectContext* NewContext = new FARPGGameplayEffectContext();
		*NewContext = *this;

		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		// 반드시 자신의 스크립트 구조체를 반환
		return FARPGGameplayEffectContext::StaticStruct();
	}
	
	/** 멀티플레이 시 이 컨텍스트가 전송될 수 있으므로, 필요한 직렬화 처리 */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	int8 ComboIndex{ INDEX_NONE };
	int8 HitReactIndex{ INDEX_NONE };

protected:
	/** Ability Source object (should implement IARPGAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;



};

/**
 * 반드시 TStructOpsTypeTraits에 NetSerialize 사용 여부를 지정해줘야
 * 엔진이 이 구조체를 네트워크에 올바르게 직렬화합니다.
 */
template<>
struct TStructOpsTypeTraits<FARPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FARPGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, // NetSerialize 오버라이드 사용
		WithCopy = true          // Duplicate() 사용
	};
};