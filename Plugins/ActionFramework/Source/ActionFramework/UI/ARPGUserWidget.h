// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPGUserWidget.generated.h"

/**
 * 
 */
class UARPGPresenter;

UCLASS()
class ACTIONFRAMEWORK_API UARPGUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetPresenter(UARPGPresenter* NewPresenter);

	FORCEINLINE UARPGPresenter* GetPresenter() { return Presenter; }
protected:
	virtual void WidgetPresenterSet();
private:
	TObjectPtr<UARPGPresenter> Presenter;
};
