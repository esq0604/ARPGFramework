// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/AbilitySystem/ARPGAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ActionFramework/UI/ARPGHUD.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "ActionFramework/Player/ARPGPlayerState.h"

bool UARPGAbilitySystemLibrary::MakeWidgetPresenterParams(const UObject* WorldContextObject, FPresenterParams& OutWCParams, AARPGHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AARPGHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AARPGPlayerState* PS = PC->GetPlayerState<AARPGPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystem = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayPresenter* UARPGAbilitySystemLibrary::GetOverlayWidgetPresenter(const UObject* WorldContextObject)
{
	FPresenterParams PresenterParams;
	AARPGHUD* HUD = nullptr;
	if (MakeWidgetPresenterParams(WorldContextObject, PresenterParams, HUD))
	{
		return HUD->GetOverlayPresenter(PresenterParams);
	}

	return nullptr;
}
