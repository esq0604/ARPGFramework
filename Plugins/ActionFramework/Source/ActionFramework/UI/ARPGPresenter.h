// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ARPGPresenter.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
class UARPGUserWidget;
class UEquipmentPresenterMediator;
USTRUCT()
struct FPresenterParams
{
	GENERATED_BODY()
	FPresenterParams() {}

	FPresenterParams(APlayerController* PC , APlayerState* PS, UAbilitySystemComponent* ASC , UAttributeSet* AS, UActorComponent* AC)
		:PlayerController(PC), PlayerState(PS), AbilitySystem(ASC), AttributeSet(AS), OptionalActorComponent(AC) {}


	TObjectPtr<APlayerController> PlayerController{ nullptr };

	TObjectPtr<APlayerState> PlayerState{ nullptr };

	TObjectPtr<UAbilitySystemComponent> AbilitySystem{ nullptr };

	TObjectPtr<UAttributeSet> AttributeSet{ nullptr };

	TObjectPtr<UActorComponent> OptionalActorComponent{ nullptr };

};

USTRUCT()
struct FSubWidgetPrsenter
{
	GENERATED_BODY()

	FSubWidgetPrsenter() {}

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UARPGUserWidget> Widget;

	UPROPERTY(EditAnywhere)
	FName SubWidgetWidgetName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UARPGPresenter> Presenter;
};

UCLASS(BlueprintType,Blueprintable)
class ACTIONFRAMEWORK_API UARPGPresenter : public UObject 
{
	GENERATED_BODY()
	
public:
	void SetPresenterParams(const FPresenterParams& PresenterParams);
	void SetView(UARPGUserWidget* NewView);

	UEquipmentPresenterMediator* GetPresenterMediator() { return PresenterMediator;}
	UARPGUserWidget* GetView() { return View; }

	virtual void BroadcastInitialValues();
	virtual void BindCallBacksToDependencies();
	
	FORCEINLINE APlayerController* GetPlayerController() { return PlayerController; }
	FORCEINLINE APlayerState* GetPlayerState() { return PlayerState; }
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() { return AbilitySystemComponent; }
	FORCEINLINE UAttributeSet* GetAttributeSet() { return AttributeSet; }
	FORCEINLINE UActorComponent* GetOptionalActorComponent() { return OptionalActorComponent; }
private:

	TObjectPtr<APlayerController> PlayerController;

	TObjectPtr<APlayerState> PlayerState;

	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TObjectPtr<UAttributeSet> AttributeSet;

	//Model이 다른 컴포넌트로 구성되어있다면 해당 변수를 사용합니다.
	TObjectPtr<UActorComponent> OptionalActorComponent;

	TObjectPtr<UARPGUserWidget> View;


	UPROPERTY(EditDefaultsOnly, Category = "SubWidget")
	bool bHasSubWidget;
	//위젯
	UPROPERTY(EditDefaultsOnly, Category = "SubWidget", meta = (EditCondition = "bHasSubWidget"))
	TArray<FSubWidgetPrsenter> SubWidgetInfos;

	UPROPERTY(EditDefaultsOnly, Category = "SubWidget", meta = (EditCondition = "bHasSubWidget"))
	TSubclassOf<UEquipmentPresenterMediator> PresenterMediatorClass;
	TObjectPtr<UEquipmentPresenterMediator> PresenterMediator;


};
