// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EquipmentViewModel.h"
#include "Components/Image.h"


UEquipmentViewModel::UEquipmentViewModel()
{
}

FText UEquipmentViewModel::GetItemName() const
{
    return ItemName;
}

void UEquipmentViewModel::SetItemName(FText NewItemName)
{
    if (UE_MVVM_SET_PROPERTY_VALUE(ItemName, NewItemName))
    {
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetItemName);
    }
}
