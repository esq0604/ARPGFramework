// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/OverlayPresenter.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"


void UOverlayPresenter::BroadcastInitialValues()
{
	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	if (AT)
	{
		UpdateHealthPercent(AT->GetHealth(), AT->GetMaxHealth());
	}
	else
	{
	}
}

void UOverlayPresenter::BindCallBacksToDependencies()
{
	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetHealthAttribute()).AddUObject(this, &UOverlayPresenter::HealthChanged);
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetMaxHealthAttribute()).AddUObject(this, &UOverlayPresenter::MaxHealthChanged);

}

void UOverlayPresenter::HealthChanged(const FOnAttributeChangeData& Data)
{	

	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	UpdateHealthPercent(Data.NewValue, AT->GetMaxHealth());
}

void UOverlayPresenter::MaxHealthChanged(const FOnAttributeChangeData& Data)
{

	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	UpdateHealthPercent(AT->GetHealth(), Data.NewValue);
	
}

// Updates the health percentage based on current health and max health.
void UOverlayPresenter::UpdateHealthPercent(float NewHealth, float NewMaxHealth)
{
	if (NewHealth > 0.0f)
	{
		float Percent = NewHealth / NewMaxHealth;

		OnHealthPercentChange.Broadcast(Percent); // Notify View about health percentage update
	}
	else
	{
		OnHealthPercentChange.Broadcast(0.0f); // Avoid division by zero
	}
}
