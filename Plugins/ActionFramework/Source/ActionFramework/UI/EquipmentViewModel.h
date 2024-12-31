// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "EquipmentViewModel.generated.h"

/**
 * 
 */
class UImage;

UCLASS()
class ACTIONFRAMEWORK_API UEquipmentViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UEquipmentViewModel();

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetItemName() const;

	UFUNCTION()
	void SetItemName(FText NewItemName);
private:
	UPROPERTY(FieldNotify,Setter="SetItemName", Getter)
	FText ItemName;

};
