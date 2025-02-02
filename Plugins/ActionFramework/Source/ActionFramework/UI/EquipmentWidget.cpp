// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/UI/ItemListPanel.h"
#include "ActionFramework/UI/SlotWrapperWidget.h"
#include "ActionFramework/UI/ItemDescriptionPanel.h"
#include "ActionFramework/UI/EquipmentPresenter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"

void UEquipmentWidget::NativeConstruct()
{
    Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UEquipmentWidget::HandleCloseButtonClicked);
	}


    InitSlotInPanel(WeaponPanel);
    InitSlotInPanel(ArmorPanel);
    InitSlotInPanel(ToolPanel);

}

void UEquipmentWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveDynamic(this, &UEquipmentWidget::HandleCloseButtonClicked);
    }

    for (UWidget* Widget : WeaponPanel->GetAllChildren())
    {
        USlot* CategorySlot = Cast<USlot>(Widget);
        if (CategorySlot)
        {
            
            CategorySlot->OnSlotClicked.RemoveDynamic(this, &UEquipmentWidget::OnCategorySlotClicked);
        }
    }
    for (UWidget* Widget : ArmorPanel->GetAllChildren())
    {
        USlot* CategorySlot = Cast<USlot>(Widget);
        if (CategorySlot)
        {
            CategorySlot->OnSlotClicked.RemoveDynamic(this, &UEquipmentWidget::OnCategorySlotClicked);
        }
    }
    for (UWidget* Widget : ToolPanel->GetAllChildren())
    {
        USlot* CategorySlot = Cast<USlot>(Widget);
        if (CategorySlot)
        {
            CategorySlot->OnSlotClicked.RemoveDynamic(this, &UEquipmentWidget::OnCategorySlotClicked);
        }
    }
}

void UEquipmentWidget::ShowItemListPanel()
{
    ItemListPanel->SetVisibility(ESlateVisibility::Visible);
    ItemCategoryPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UEquipmentWidget::HandleCloseButtonClicked()
{
    // ItemListPanel 숨기기
    if (ItemListPanel)
    {
        ItemListPanel->SetVisibility(ESlateVisibility::Collapsed);
    }

    // WrapperPanel 보이기
    if (ItemCategoryPanel)
    {
        ItemCategoryPanel->SetVisibility(ESlateVisibility::Visible);
    }

    //RemoveFromParent();
    
    this->SetVisibility(ESlateVisibility::Collapsed);
}

void UEquipmentWidget::OnCategorySlotClicked(USlot* ClickedSlot)
{
    OnCategorySlotClickedDelegate.Broadcast(ClickedSlot->GetSlotType(), ClickedSlot->SlotIndex);
}

void UEquipmentWidget::OnCategorySlotEntered(USlot* EnteredSlot)
{
    OnCategorySlotEnteredDelegate.Broadcast(EnteredSlot->GetSlotType(), EnteredSlot->SlotIndex);
}

void UEquipmentWidget::InitSlotInPanel(UUniformGridPanel* SlotPanel)
{
    TArray<UWidget*> Widgets = SlotPanel->GetAllChildren();
    for (int idx = 0; idx < Widgets.Num(); idx++)
    {
        
        USlot* CategorySlot = Cast<USlot>(Widgets[idx]);
        if (CategorySlot)
        {
            SlotTagMap.FindOrAdd(CategorySlot->GetSlotType()).Add(CategorySlot->SlotIndex, CategorySlot);
            //CategorySlot->SlotIndex=idx;
            CategorySlot->OnSlotMouseEnter.AddDynamic(this, &UEquipmentWidget::OnCategorySlotEntered);
            CategorySlot->OnSlotClicked.AddDynamic(this, &UEquipmentWidget::OnCategorySlotClicked);
           //SlotTagMap.Add(CategorySlot, CategorySlot->GetSlotType());
        }
        
    }
}

void UEquipmentWidget::UpdateSlotNameTextBlock(FGameplayTag ItemTypeTag, uint8 Index)
{
    USlot* FoundSlot = FindSlotAtSlotTagMap(ItemTypeTag, Index);

    if (FoundSlot)
    {

        SlotName_TextBlock->SetText(FoundSlot->SlotNameText);
    }
}

void UEquipmentWidget::UpdateStoreItemNameTextBlock(FGameplayTag ItemTypeTag, uint8 Index)
{
    USlot* FoundSlot = FindSlotAtSlotTagMap(ItemTypeTag, Index);
    if (FoundSlot)
    {
        StoreItemName_TextBlock->SetText(FoundSlot->StoreItemText);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChangeStoreItemNameTextBlock , -, "));
        StoreItemName_TextBlock->SetText(FText::FromString("-"));
    }
}

USlot* UEquipmentWidget::FindSlotAtSlotTagMap(FGameplayTag ItemTypeTag, uint8 Index)
{
    if (SlotTagMap.Contains(ItemTypeTag))
    {
        const TMap<uint8, TObjectPtr<USlot>>& IndexMap = SlotTagMap[ItemTypeTag];
        USlot* FoundSlot = IndexMap.FindRef(Index);
        if (FoundSlot)
        {
            return FoundSlot;
        }
    }
    return nullptr;
}


void UEquipmentWidget::UpdateSlot(FGameplayTag ItemTypeTag, uint8 Index, const FSlotDisplayInfo& SlotInfo)
{ 
    USlot* FoundSlot =  FindSlotAtSlotTagMap(ItemTypeTag, Index);
    if (FoundSlot)
    {
        UE_LOG(LogTemp, Warning, TEXT("FindSlot , Do EquipmentWidget::UpdateSlot"));
        FoundSlot->UpdateSlot(SlotInfo);
    }
    else
    {

        UE_LOG(LogTemp, Warning, TEXT("CanNot FindSlot , Do EquipmentWidget::UpdateSlot Fail"));
    }
    ItemCategoryPanel->SetVisibility(ESlateVisibility::Visible);

}

void UEquipmentWidget::WidgetPresenterSet()
{
    UEquipmentPresenter* EquipPresenter = Cast<UEquipmentPresenter>(GetPresenter());

    //ModelDataChangeEvent
    EquipPresenter->OnEquipItemEvent.AddDynamic(this, &UEquipmentWidget::UpdateSlot);
    
    //MouseEvent
    EquipPresenter->OnCategorySlotMouseEntered.AddDynamic(this, &UEquipmentWidget::UpdateSlotNameTextBlock);
    EquipPresenter->OnCategorySlotMouseEntered.AddDynamic(this, &UEquipmentWidget::UpdateStoreItemNameTextBlock);
}


