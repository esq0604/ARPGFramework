// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ActionFramework/Datas/ItemBaseDataAsset.h"
#include "ActionFramework/Interface/Equipable.h"
#include "ActionFramework/Interface/Useable.h"
#include "ActionFramework/UI/InventoryWidget.h"
#include "ActionFramework/UI/EquipmentWidget.h"
#include "ActionFramework/UI/QuickSlotWidget.h"
#include "ActionFramework/UI/Slot.h"
#include "ActionFramework/Items/WeaponItem.h"
#include "ActionFramework/UI/SlotViewModel.h"
#include "MVVMGameSubSystem.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	ItemContainer.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")), TArray<TObjectPtr<UItemBaseDataAsset>>());

	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")), TArray<TObjectPtr<UItemBaseDataAsset>>());
	CurEquipedItemData.Add(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")), TArray<TObjectPtr<UItemBaseDataAsset>>());

	// 각 태그에 대해 초기화
	for (auto& Pair : ItemContainer)
	{
		Pair.Value.Init(nullptr, ItemContainerSize);
	}

	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Weapon")))->Init(nullptr, 3);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Armor")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Helmet")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Gloves")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Equipment.Shoes")))->Init(nullptr, 1);
	CurEquipedItemData.Find(FGameplayTag::RequestGameplayTag(FName("ItemType.Tool")))->Init(nullptr, 6);



}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AddStartingItem();

}

void UInventoryComponent::ToggleInventoryWidget()
{
	if (!InventoryWidget && InventoryWidgetClass)
	{

		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->OnCloseButtonClicked.AddDynamic(this, &UInventoryComponent::ToggleInventoryWidget);
		InventoryWidget->OnItemTypeSelected.AddDynamic(this, &UInventoryComponent::UpdateItemList);

		// 처음 생성 시에만 AddToViewport 호출
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);  // 초기 상태는 숨김
	}

	if (InventoryWidget)
	{
		// Visibility를 기반으로 UI 토글
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetMouseCursorVisibility(false);

		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryComponent::ToggleEquipmentWidget()
{
	if (!EquipmentWidget && EquipmentWidgetClass)
	{
		EquipmentWidget = CreateWidget<UEquipmentWidget>(GetWorld(), EquipmentWidgetClass);
		////EquipmentWidget
		//EquipmentWidget->OnCloseButtonClicked.AddDynamic(this, &UInventoryComponent::ToggleEquipmentWidget);
		//EquipmentWidget->OnRequestItemList.AddDynamic(this, &UInventoryComponent::UpdateItemList);
		//EquipmentWidget->OnRequestEquipItem.AddDynamic(this, &UInventoryComponent::EquipUnEquipItemToEquipment);

		/*EquipmentViewModel = NewObject<UEquipmentViewModel>();
		if (EquipmentViewModel)
		{
			
		}*/
		
		
		// 처음 생성 시에만 AddToViewport 호출
		EquipmentWidget->AddToViewport();
		EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);  // 초기 상태는 숨김
	}

	if (EquipmentWidget)
	{
		// Visibility를 기반으로 UI 토글
		if (EquipmentWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetMouseCursorVisibility(false);
		}
		else
		{
			EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryComponent::ToggleQuickSlotWidget()
{

}

UItemBaseDataAsset* UInventoryComponent::GetCurrentEquipWeaponData()
{
	if (CurEquipedItemData[FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon")][CurUsingWeaponIndex] != nullptr)
	{
		return CurEquipedItemData[FGameplayTag::RequestGameplayTag("ItemType.Equipment.Weapon")][CurUsingWeaponIndex];
	}

	return nullptr;
}


void UInventoryComponent::AddStartingItem()
{
	for (const UItemBaseDataAsset* Item : StartingItems)
	{

		const UItemBaseDataAsset* DuplicatedItemDataAsset = DuplicateObject(Item, GetOwner());
		
		if (DuplicatedItemDataAsset != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DuplicatedItemDataAsset->GetOuter()->GetName());
			AddItemToItemContainer(DuplicatedItemDataAsset);
			AddItemToQuickSlotWidget(DuplicatedItemDataAsset);
		}
	}
}

void UInventoryComponent::AddItemToItemContainer(const UItemBaseDataAsset* AddedItem)
{
		if (!AddedItem)
			return;

		FGameplayTag ItemTypeTag = AddedItem->ItemTypeTag;

		if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemTypeTag))
		{
			int32 EmptySlotIndex = Items->IndexOfByKey(nullptr);
			// 1. 빈 슬롯(Null) 검색 및 아이템 추가
			for (int32 i = 0; i < Items->Num(); ++i)
			{
				if ((*Items)[i] == nullptr)
				{
					(*Items)[i] = const_cast<UItemBaseDataAsset*>(AddedItem);
					break;
				}
			}
		}
}

void UInventoryComponent::AddItemToQuickSlotWidget(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::EquipItem(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::UnEquipItem(const UItemBaseDataAsset* AddedItem)
{
}

void UInventoryComponent::UseItem(const UItemBaseDataAsset* AddedItem)
{
}

//AActor* UInventoryComponent::SpawnedItemFromDataAsset(const UItemBaseDataAsset* InData)
//{
//	AItemBase* SpawnItem = nullptr;
//	if (InData == nullptr)
//		return nullptr;
//	if (InData->ActorToSpawnClass == nullptr)
//		return nullptr;
//
//	const FTransform SpawnTransform = GetOwner()->GetActorTransform();
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.Owner = GetOwner();
//	SpawnItem = GetWorld()->SpawnActor<AItemBase>(InData->ActorToSpawnClass, SpawnTransform, SpawnInfo);
//
//	return nullptr;
//}

void UInventoryComponent::EquipUnEquipItemToEquipment(FGameplayTag ItemType, int32 ContainerIndex , int32 EquipIndex)
{

	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemType))
	{
		// 2. 유효한 ContainerIndex 확인
		if (Items->IsValidIndex(ContainerIndex))
		{
			UItemBaseDataAsset* SelectedItem = (*Items)[ContainerIndex];
			if (SelectedItem)
			{
				TArray<TObjectPtr<UItemBaseDataAsset>>* EquipItems = CurEquipedItemData.Find(ItemType);
				// 같은 아이템 클릭시 기존 장비 해제
				if (EquipItems && EquipItems->IsValidIndex(EquipIndex))
				{
					UItemBaseDataAsset* CurrentlyEquippedItem = (*EquipItems)[EquipIndex];
					if (CurrentlyEquippedItem && CurrentlyEquippedItem->Implements<UEquipable>()) 
					{
						UE_LOG(LogTemp, Warning, TEXT("UnEquip"));
						IEquipable* EquipableItem = Cast<IEquipable>(CurrentlyEquippedItem);
						EquipableItem->UnEquip();
						(*EquipItems)[EquipIndex] = nullptr;

						if (EquipmentWidget)
						{
							FSlotDisplayInfo SlotInfo;
							SlotInfo.Icon = nullptr; // 또는 빈 아이콘 리소스 설정
							SlotInfo.Name = FText::FromString(TEXT("")); // 빈 텍스트
							SlotInfo.Count = 0; // 카운트는 0
							EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
							return;
						}
					}
				}

				// 다른 아이템 클릭시 새 장비 장착
				if (EquipItems) 
				{
					(*EquipItems)[EquipIndex] = SelectedItem;
					if (SelectedItem->Implements<UEquipable>())
					{
						IEquipable* EquipableItem = Cast<IEquipable>(SelectedItem);
						EquipableItem->Equip();
					}
					// 4. EquipmentWidget 업데이트
					if (EquipmentWidget)
					{
						FSlotDisplayInfo SlotInfo;
						SlotInfo.Icon = SelectedItem->AssetData.Icon;
						SlotInfo.Name = SelectedItem->TextData.Name;
						SlotInfo.Count = SelectedItem->NumericData.Quantity;

						EquipmentWidget->UpdateEquipRequestSlot(SlotInfo);
					}
				}
			}
		}
	}
}


void UInventoryComponent::UpdateItemList(FGameplayTag ItemTypeTag, UUserWidget* RequestingWidget)
{
	//ItemContainer에서 ItemTypeTag에 맞는 아이템 배열을 가져와서 InventoryWidget을 Update하도록합니다.
		// ItemContainer에서 ItemTypeTag에 해당하는 아이템 배열을 가져옴
	TArray<FSlotDisplayInfo> Infos;

	if (TArray<TObjectPtr<UItemBaseDataAsset>>* Items = ItemContainer.Find(ItemTypeTag))
	{
		for (const auto& Item : *Items)
		{
			if (Item)
			{
				Infos.Add(FSlotDisplayInfo(Item->AssetData.Icon, Item->NumericData.Quantity, Item->TextData.Name));
			}
		}
		// 호출한 위젯이 InventoryWidget인 경우
		if (UInventoryWidget* InventoryWidgetInstance = Cast<UInventoryWidget>(RequestingWidget))
		{
			InventoryWidgetInstance->UpdateItemListPanel(Infos);
			//InventoryWidgetInstance->SetCurOpenItemListPanel(ItemTypeTag);
		}
		// 호출한 위젯이 EquipmentWidget인 경우
		else if (UEquipmentWidget* EquipmentWidgetInstance = Cast<UEquipmentWidget>(RequestingWidget))
		{
			EquipmentWidgetInstance->UpdateItemListPanel(Infos);
			EquipmentWidgetInstance->SetCurOpenItemListPanel(ItemTypeTag);
		}
	}
}

void UInventoryComponent::UpdateEquipmentWiget(FGameplayTag ItemTypeTag)
{
}

void UInventoryComponent::SetMouseCursorVisibility(bool bIsVisible)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetShowMouseCursor(bIsVisible);
	}
}




