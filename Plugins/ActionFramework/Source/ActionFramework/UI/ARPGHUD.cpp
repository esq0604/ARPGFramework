// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ARPGHUD.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "ActionFramework/UI/EscMenuWidget.h"
#include "ActionFramework/UI/ARPGPresenter.h"
#include "ActionFramework/ARPGGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

UARPGPresenter* AARPGHUD::GetPresenter(const FPresenterParams& PresenterParams, TSubclassOf<UARPGPresenter> PresenterClass)
{
	UARPGPresenter* ReturnValue = nullptr;
	if (PresenterClass != nullptr)
	{
		ReturnValue = NewObject<UARPGPresenter>(this, PresenterClass);
		ReturnValue->SetPresenterParams(PresenterParams);
		ReturnValue->BindCallBacksToDependencies();
		return ReturnValue;
	}
	return ReturnValue;
}

void AARPGHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AARPGHUD::InitEquipmentWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, UActorComponent* AC)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), EquipmentWidgetClass);
	EquipmentWidget = Cast<UARPGUserWidget>(Widget);

	FPresenterParams PresenterParams(PC, PS, ASC, AS, AC);
	EquipmentPresenter = GetPresenter(PresenterParams, EquipmentPresenterClass);
	EquipmentPresenter->SetView(EquipmentWidget);
	EquipmentPresenter->BindCallBacksToDependencies();
	EquipmentWidget->SetPresenter(EquipmentPresenter);
}

void AARPGHUD::InitQuickWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, UActorComponent* AC)
{

}

void AARPGHUD::InitOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (OverlayWidgetClass && OverlayPresenter)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget = Cast<UARPGUserWidget>(Widget);

		FPresenterParams PresenterParams(PC, PS, ASC, AS, nullptr);
		OverlayPresenter = GetPresenter(PresenterParams, OverlayPresenterClass);
		OverlayWidget->SetPresenter(OverlayPresenter);
		OverlayWidget->AddToViewport();
		OverlayPresenter->BroadcastInitialValues();
	}
}

void AARPGHUD::InitMenuWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (EscWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), EscWidgetClass);
		EscWidget = Cast<UEscMenuWidget>(Widget);
		EscWidget->OnMenuButtonClicked.AddDynamic(this, &AARPGHUD::ToggleSelectMenuWidget);
		//FPresenterParams PresenterParams(PC, PS, ASC, AS, nullptr);
		//EscPresenter = GetPresenter(PresenterParams, EscPresenterClass);
		//EscWidget->SetPresenter(EquipmentPresenter);
	}
}

bool AARPGHUD::ToggleMenuWidget()
{
	if (!EscWidget)
	{
		return false;
	}
	if (!EscWidget->IsInViewport())
	{
		EscWidget->AddToViewport();
		return true;
	}

	if (EscWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		EscWidget->SetVisibility(ESlateVisibility::Visible);
		return true;
	}

	
	//EscWidget->RemoveFromParent();
	EscWidget->SetVisibility(ESlateVisibility::Collapsed);
	return true;
}

void AARPGHUD::ToggleSelectMenuWidget(FGameplayTag EventTag)
{
	if (EventTag.MatchesTag(ARPGGameplayTags::GameplayEvent_OpenEquipment))
	{
		if (EquipmentWidget)
		{
			if (!EquipmentWidget->IsInViewport())
			{
				EquipmentWidget->AddToViewport();
				//EquipmentPresenter->BroadcastInitialValues();
			}
			
			if (EquipmentWidget->GetVisibility() == ESlateVisibility::Collapsed)
			{
				EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}

		ToggleMenuWidget();
	}
}
