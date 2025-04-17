// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/UI/OverlayPresenter.h"
#include "ActionFramework/AbilitySystem/ARPGAttributeSet.h"
#include "ActionFramework/UI/OverlayWidget.h"
#include "AbilitySystemInterface.h"


void UOverlayPresenter::BroadcastInitialValues()
{
	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	if (AT)
	{
        PlayerUpdateHealthPercent(AT->GetHealth(), AT->GetMaxHealth());
        PlayerUpdatePosturePercent(AT->GetPosture(), AT->GetMaxPosture());
	}
	else
	{
	}
}

void UOverlayPresenter::BindCallBacksToDependencies()
{
	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetHealthAttribute()).AddUObject(this, &UOverlayPresenter::PlayerHealthChanged);
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetMaxHealthAttribute()).AddUObject(this, &UOverlayPresenter::PlayerMaxHealthChanged);
    GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetPostureAttribute()).AddUObject(this, &UOverlayPresenter::PlayerPostureChanged);
    GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AT->GetMaxPostureAttribute()).AddUObject(this, &UOverlayPresenter::PlayerMaxPostureChanged);

}

void UOverlayPresenter::SetBoss(AActor* NewBoss)
{
    UE_LOG(LogTemp, Warning, TEXT("SetBoss In Presenter"));
    // 1) 이전 보스 Delegate 해제
    if (BossActor)
    {
        BossActor = NewBoss;
        if (IAbilitySystemInterface* NewASI = Cast<IAbilitySystemInterface>(BossActor))
        {
            if (UAbilitySystemComponent* NewASC = NewASI->GetAbilitySystemComponent())
            {
                BossASC = NewASC;
                if (const UARPGAttributeSet* NewAttr = Cast<const UARPGAttributeSet>(
                    NewASC->GetAttributeSet(UARPGAttributeSet::StaticClass())))
                {
                    NewASC->GetGameplayAttributeValueChangeDelegate(NewAttr->GetHealthAttribute()).RemoveAll(this);
                    NewASC->GetGameplayAttributeValueChangeDelegate(NewAttr->GetPostureAttribute()).RemoveAll(this);
                }
            }
        }
    }
    else
    {
        BossActor = NewBoss;
    }

    // 2) 보스 정보를 갱신
   

    //보스가 바뀌면 처리해야할거같음
    // 3) 보스가 없으면 → UI 숨김
    if (BossActor == nullptr)
    {
        // e.g. OnBossHealthPercentChange.Broadcast(0.f);
        // BossPosturePercent 등도 0.f
        return;
    }

    // 4) 새 보스가 유효하면 Delegate 바인딩
    if (IAbilitySystemInterface* NewASI = Cast<IAbilitySystemInterface>(BossActor))
    {
        if (UAbilitySystemComponent* NewASC = NewASI->GetAbilitySystemComponent())
        {
            BossASC = NewASC;

            if (const UARPGAttributeSet* NewAttr = Cast<const UARPGAttributeSet>(
                NewASC->GetAttributeSet(UARPGAttributeSet::StaticClass())))
            {
                NewASC->GetGameplayAttributeValueChangeDelegate(NewAttr->GetHealthAttribute()).AddUObject(this, &UOverlayPresenter::BossHealthChanged);
                NewASC->GetGameplayAttributeValueChangeDelegate(NewAttr->GetPostureAttribute()).AddUObject(this, &UOverlayPresenter::BossPostureChanged);

                UOverlayWidget* OverlayWidget = Cast<UOverlayWidget>(GetView());
                if (OverlayWidget)
                {
                    //OnBossNameChange.BroadCast(BossActor->)
                    BossUpdateHealthPercent(NewAttr->GetHealth(),NewAttr->GetMaxHealth());
                    BossUpdatePosturePercent(NewAttr->GetPosture(), NewAttr->GetMaxPosture());
                }
            }
        }
    }
}

void UOverlayPresenter::PlayerHealthChanged(const FOnAttributeChangeData& Data)
{	

	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	PlayerUpdateHealthPercent(Data.NewValue, AT->GetMaxHealth());
}

void UOverlayPresenter::PlayerMaxHealthChanged(const FOnAttributeChangeData& Data)
{

	UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
	PlayerUpdateHealthPercent(AT->GetHealth(), Data.NewValue);
	
}

// Updates the health percentage based on current health and max health.
void UOverlayPresenter::PlayerUpdateHealthPercent(float NewHealth, float NewMaxHealth)
{
	if (NewHealth > 0.0f)
	{
		float Percent = NewHealth / NewMaxHealth;

		OnPlayerHealthPercentChange.Broadcast(Percent); // Notify View about health percentage update
	}
	else
	{
		OnPlayerHealthPercentChange.Broadcast(0.0f); // Avoid division by zero
	}
}

void UOverlayPresenter::PlayerPostureChanged(const FOnAttributeChangeData& Data)
{
    UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
    PlayerUpdatePosturePercent(Data.NewValue, AT->GetMaxPosture());

}

void UOverlayPresenter::PlayerMaxPostureChanged(const FOnAttributeChangeData& Data)
{
    UARPGAttributeSet* AT = Cast<UARPGAttributeSet>(GetAttributeSet());
    PlayerUpdatePosturePercent(AT->GetHealth(), Data.NewValue);

}

void UOverlayPresenter::PlayerUpdatePosturePercent(float NewPosture, float NewMaxPosture)
{
    if (NewPosture > 0.0f)
    {
        float Percent = NewPosture / NewMaxPosture;

        OnPlayerPosturePercentChange.Broadcast(Percent); // Notify View about health percentage update
    }
    else
    {
        OnPlayerPosturePercentChange.Broadcast(0.0f); // Avoid division by zero
    }
}

void UOverlayPresenter::BossHealthChanged(const FOnAttributeChangeData& Data)
{
    if (const UARPGAttributeSet* BossAT = Cast<const UARPGAttributeSet>(
        BossASC->GetAttributeSet(UARPGAttributeSet::StaticClass())))
    {
        BossUpdateHealthPercent(Data.NewValue , BossAT->GetMaxHealth());
    }
}

void UOverlayPresenter::BossPostureChanged(const FOnAttributeChangeData& Data)
{
    if (const UARPGAttributeSet* BossAT = Cast<const UARPGAttributeSet>(
        BossASC->GetAttributeSet(UARPGAttributeSet::StaticClass())))
    {
        BossUpdatePosturePercent(Data.NewValue, BossAT->GetMaxPosture());
    }
}

void UOverlayPresenter::BossUpdateHealthPercent(float NewHealth, float NewMaxHealth)
{
    if (NewHealth > 0.0f)
    {
        float Percent = NewHealth / NewMaxHealth;

        OnBossHealthPercentChange.Broadcast(Percent); // Notify View about health percentage update
    }
    else
    {
        OnBossHealthPercentChange.Broadcast(0.0f); // Avoid division by zero
    }
}

void UOverlayPresenter::BossUpdatePosturePercent(float NewPosture, float NewMaxPosture)
{
    if (NewPosture > 0.0f)
    {
        float Percent = NewPosture / NewMaxPosture;

        OnBossPosturePercentChange.Broadcast(Percent); // Notify View about health percentage update
    }
    else
    {
        OnBossPosturePercentChange.Broadcast(0.0f); // Avoid division by zero
    }
}

//void UOverlayPresenter::BossNameChanged(const FOnAttributeChangeData& Data)
//{
//}
