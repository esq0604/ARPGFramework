// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/Datas/ItemBaseDataAsset.h"


AActor* UItemBaseDataAsset::GetSpawnedActor()
{
	if (SpawnedActor != nullptr)
	{
		return  SpawnedActor;
	}
	else
		return nullptr;
}
