// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerState.h"
#include "AbilitySystemComponent.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"


AARPGPlayerState::AARPGPlayerState()
{
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AttributeSet = CreateDefaultSubobject<UARPGAttributeSet>(TEXT("ARPGAttributeSet"));
}

UAbilitySystemComponent* AARPGPlayerState::GetAbilitySystemComponent() const
{
    if (ASC)
    {
        return ASC;
    }

    UE_LOG(LogTemp, Warning, TEXT("AARPGPlayerState ASC nullptr"));
    return nullptr;
}

void AARPGPlayerState::BeginPlay()
{
    Super::BeginPlay();

}

