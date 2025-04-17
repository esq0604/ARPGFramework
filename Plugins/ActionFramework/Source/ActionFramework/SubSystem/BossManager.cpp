// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/SubSystem/BossManager.h"
#include "Kismet/GameplayStatics.h"
#include "ActionFramework/GameModes/ARPGGameMode.h"
#include "BossManager.h"

void UBossManager::SetBoss(APawn* NewBoss)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);

	if (GameMode)
	{
		AARPGGameMode* ARPGGM = Cast<AARPGGameMode>(GameMode);
		if (ARPGGM)
		{
			//ARPGGM-
		}
	}
}
