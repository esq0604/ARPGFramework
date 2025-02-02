// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "ARPGHUD.generated.h"

/**
 * 
 */
class UARPGUserWidget;
class UARPGPresenter;
class UAbilitySystemComponent;
class UAttributeSet;
class UEscMenuWidget;
struct FPresenterParams;

UCLASS()
class ACTIONFRAMEWORK_API AARPGHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UARPGPresenter* GetPresenter(const FPresenterParams& PresenterParams, TSubclassOf <UARPGPresenter> PresenterClass);

	void InitEquipmentWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, UActorComponent* AC);
	void InitQuickWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, UActorComponent* AC);
	void InitOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitMenuWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	bool ToggleMenuWidget();

	UFUNCTION()
	void ToggleSelectMenuWidget(FGameplayTag EventTag);
protected:
	virtual void BeginPlay() override;

private:


public:
	TObjectPtr<UEscMenuWidget> EscWidget;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UARPGUserWidget> InventoryWidgetClass;
	TObjectPtr<UARPGUserWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> EquipmentWidgetClass;
	TObjectPtr<UARPGUserWidget> EquipmentWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> QuickWidgetClass;
	TObjectPtr<UARPGUserWidget> QuickWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	TObjectPtr<UARPGUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> EscWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UARPGPresenter> EquipmentPresenterClass;
	TObjectPtr<UARPGPresenter> EquipmentPresenter;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UARPGPresenter> ItemListPanelPresenterClass;
	//TObjectPtr<UARPGPresenter> ItemListPanelPresenter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UARPGPresenter> OverlayPresenterClass;
	TObjectPtr<UARPGPresenter> OverlayPresenter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UARPGPresenter> EscPresenterClass;
	TObjectPtr<UARPGPresenter> EscPresenter;

};
