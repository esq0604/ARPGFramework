// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ActionFramework/Input/ARPGInputConfig.h"
#include "ARPGInputComponent.generated.h"

/**
 * 
 */
class UARPGInputConfig;

UCLASS()
class ACTIONFRAMEWORK_API UARPGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UARPGInputComponent();

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UARPGInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void

};
