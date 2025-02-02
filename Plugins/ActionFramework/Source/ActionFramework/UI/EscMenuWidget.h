// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFramework/UI/ARPGUserWidget.h"
#include "GameplayTagContainer.h"
#include "EscMenuWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuButtonClicked, FGameplayTag, ButtonTag);

class UButton;
UCLASS()
class ACTIONFRAMEWORK_API UEscMenuWidget : public UARPGUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY()
	FOnMenuButtonClicked OnMenuButtonClicked;
private:
	virtual void WidgetPresenterSet() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> InventoryButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipmentButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UFUNCTION()
	void HandleInventoryButtonClicked();

	UFUNCTION()
	void HandleEquipmentButtonClicked();

	UFUNCTION()
	void HandleExitButtonClicked();
};
