// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPGAbilitySystemLibrary.generated.h"

/**
 * 
 */
class UOverlayPresenter;
struct FPresenterParams;
class AARPGHUD;

UCLASS()
class ACTIONFRAMEWORK_API UARPGAbilitySystemLibrary : public UObject
{
	GENERATED_BODY()

	/*
 * Widget Controller
 */

public:
	UFUNCTION(Category = "AbilitySystemLibrary|WidgetController")
	static bool MakeWidgetPresenterParams(const UObject* WorldContextObject, FPresenterParams& OutWCParams, AARPGHUD*& OutAuraHUD);

	UFUNCTION(Category = "AbilitySystemLibrary|WidgetController")
	static UOverlayPresenter* GetOverlayWidgetPresenter(const UObject* WorldContextObject);

	//UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	//static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	
};
