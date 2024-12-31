// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/ItemDescriptionPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemDescriptionPanel::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName->SetText(FText::FromString(""));
	ItemDes->SetText(FText::FromString(""));
	ItemEffect->SetText(FText::FromString(""));
}

void UItemDescriptionPanel::UpdateDescription(FDescriptionInfo NewDesInfo)
{
	ItemName->SetText(NewDesInfo.ItemName);
	ItemEffect->SetText(NewDesInfo.ItemEffect);
	ItemDes->SetText(NewDesInfo.ItemDes);
	ItemImage->SetBrushFromTexture(NewDesInfo.ItemImage);
	if (NewDesInfo.ItemImage != nullptr)
	{
		ItemImage->SetOpacity(1.f);
	}
	else
	{
		ItemImage->SetOpacity(0.f);
	}
}


