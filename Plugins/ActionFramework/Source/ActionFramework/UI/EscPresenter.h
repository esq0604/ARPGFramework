// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "GameplayTagContainer.h"
#include "EscPresenter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONFRAMEWORK_API UEscPresenter : public UARPGPresenter
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallBacksToDependencies() override;

	//void RequestOpenUI(FGameplayTag EventTag);
};
