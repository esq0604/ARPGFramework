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
			// View의 계층구조에서 위젯 검색
			UWidget* SubWidget = View->GetWidgetFromName(SubWidgetInfo.SubWidgetWidgetName);

			if (SubWidget)
			{
				// 위젯이 UUserWidget 타입인지 확인 후 캐스팅
				UARPGUserWidget* SubUserWidget = Cast<UARPGUserWidget>(SubWidget);
				if (SubUserWidget)
				{
					// Presenter 생성
					UARPGPresenter* SubWidgetPresenter = NewObject<UARPGPresenter>(this, SubWidgetInfo.Presenter);
					if (SubWidgetPresenter)
					{
						// Presenter에 View 설정
						//SetPresenterParams(GetPresen)
						FPresenterParams PresenterParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet, OptionalActorComponent);
						SubWidgetPresenter->PresenterMediator = PM;
						SubWidgetPresenter->SetPresenterParams(PresenterParams);
						SubWidgetPresenter->SetView(Cast<UARPGUserWidget>(SubUserWidget)); // 강제 캐스팅
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
