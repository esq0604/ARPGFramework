// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ARPGPresenter.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "EquipmentPresenterMediator.h"
#include "Components/Widget.h"
#include "AttributeSet.h"

void UARPGPresenter::SetPresenterParams(const FPresenterParams& PresenterParams)
{
	PlayerController = PresenterParams.PlayerController;
	PlayerState = PresenterParams.PlayerState;
	AbilitySystemComponent = PresenterParams.AbilitySystem;
	AttributeSet = PresenterParams.AttributeSet;
	OptionalActorComponent = PresenterParams.OptionalActorComponent;
}

void UARPGPresenter::SetView(UARPGUserWidget* NewView)
{
	View = NewView;

	if (SubWidgetInfos.Num() > 0)
	{
		UEquipmentPresenterMediator* PM = NewObject<UEquipmentPresenterMediator>(this, PresenterMediatorClass);
		PresenterMediator = PM;
		for (const auto& SubWidgetInfo : SubWidgetInfos)
		{
			// View�� ������������ ���� �˻�
			UWidget* SubWidget = View->GetWidgetFromName(SubWidgetInfo.SubWidgetWidgetName);

			if (SubWidget)
			{
				// ������ UUserWidget Ÿ������ Ȯ�� �� ĳ����
				UARPGUserWidget* SubUserWidget = Cast<UARPGUserWidget>(SubWidget);
				if (SubUserWidget)
				{
					// Presenter ����
					UARPGPresenter* SubWidgetPresenter = NewObject<UARPGPresenter>(this, SubWidgetInfo.Presenter);
					if (SubWidgetPresenter)
					{
						// Presenter�� View ����
						//SetPresenterParams(GetPresen)
						FPresenterParams PresenterParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet, OptionalActorComponent);
						SubWidgetPresenter->PresenterMediator = PM;
						SubWidgetPresenter->SetPresenterParams(PresenterParams);
						SubWidgetPresenter->SetView(Cast<UARPGUserWidget>(SubUserWidget)); // ���� ĳ����
						SubWidgetPresenter->BindCallBacksToDependencies();
						SubUserWidget->SetPresenter(SubWidgetPresenter);
						UE_LOG(LogTemp, Warning, TEXT("Successfully created and set SubWidgetPresenter."));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to create SubWidgetPresenter."));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("SubWidget is not a UUserWidget: %s"), *SubWidget->GetName());
				}
			}
		}
	}
}

void UARPGPresenter::BroadcastInitialValues()
{
}

void UARPGPresenter::BindCallBacksToDependencies()
{
}
