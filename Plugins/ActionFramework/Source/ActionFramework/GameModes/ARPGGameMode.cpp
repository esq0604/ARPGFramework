// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFramework/GameModes/ARPGGameMode.h"


void AARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	////게임을 로드합니다.
	//if (StartGameMenuWidgetClass)
	//{
	//	StartGameMenuWidget = CreateWidget<UStartMenu>(GetWorld(), StartGameMenuWidgetClass);
	//	StartGameMenuWidget->AddToViewport();

	//	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameAndUI());
	//	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	//	StartGameMenuWidget->StartButton->OnClicked.AddDynamic(this, &ARetargetingTestGameMode::StartNewMainGame);
	//	StartGameMenuWidget->LoadButton->OnClicked.AddDynamic(this, &ARetargetingTestGameMode::LoadMainGame);
	//	StartGameMenuWidget->EndButton->OnClicked.AddDynamic(this, &ARetargetingTestGameMode::EndGame);
	//	return;
	//}

	//for (const auto& Boss : TActorRange<ABossMonster>(GetWorld(), ABossMonster::StaticClass()))
	//{
	//	if (Boss)
	//	{
	//		Boss->OnBossDeath.AddDynamic(this, &ARetargetingTestGameMode::OnBossDeath);
	//	}
	//}

}

AARPGGameMode::AARPGGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;

}

void AARPGGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//int32 numAsset = FAssetCompilingManager::Get().GetNumRemainingAssets();
	//if (numAsset == 0)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "NumRemainingAssets Zero");
	//	SetActorTickEnabled(false);
	//	if (LoadingWidget)
	//		LoadingWidget->PlayFadeOutAnimation();
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "NumRemainingAssets");
	//}
}

void AARPGGameMode::StartNewMainGame_Implementation()
{
	//bIsGameStart = true;
	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//GameInstance->SetIsDataLoad(false);
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
	//bIsDataLoad = false;
	//StartGameMenuWidget->RemoveFromParent();
	//UGameplayStatics::OpenLevel(this, MainGameLevelName);

	//에셋컴파일이 끝난 후 이벤트를 통해 로딩씬을 닫도록합니다.


}

void AARPGGameMode::LoadMainGame()
{
	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//GameInstance->SetIsDataLoad(true);
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	//StartGameMenuWidget->RemoveFromParent();

	//UGameplayStatics::OpenLevel(this, MainGameLevelName);
}

void AARPGGameMode::FinishedShaderCompile()
{
	UE_LOG(LogTemp, Warning, TEXT("Compile Shader Finished"));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Compile Shader Finished");
}

void AARPGGameMode::EndGame()
{
	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//GameInstance->GameQuit();
}

void AARPGGameMode::OnPlayerDead()
{
	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//GameInstance->SetIsDataLoad(true);

	//UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));

}

void AARPGGameMode::LoadStartMenu()
{
	//UGameplayStatics::OpenLevel(this, StartGameLevelName);

	//StartGameMenuWidget->RemoveFromParent();
}

void AARPGGameMode::OnBossDeath()
{
	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//GameInstance->SetIsDataLoad(false);
	////if (!EndGameFadeInWidget)
	////{
	////	if (EndGameFadeInWidgetClass)
	////	{
	////		EndGameFadeInWidget = CreateWidget<UUserWidget>(GetWorld(), EndGameFadeInWidgetClass);
	////		EndGameFadeInWidget->AddToViewport();
	////	}
	////}
	//UGameplayStatics::OpenLevel(this, StartGameLevelName);

}

void AARPGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	//USGSubSystem* SG = GetGameInstance()->GetSubsystem<USGSubSystem>();

	//if (GameInstance->GetIsDataLoad())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("LoadGame InitGame"));
	//	// Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
	//	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	//	UE_LOG(LogTemp, Warning, TEXT("Save Slot Name Init Game : %s"), *SelectedSaveSlot);
	//	SG->LoadSaveGame(SelectedSaveSlot);
	//}
	//else
	//{
	//	SG->CreateSaveObject();
	//}
}

//void AARPGGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
//{
//
//	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
//
////	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
//	//if (GameInstance->GetIsDataLoad())
//	//{
//
//	//	//USGSubSystem* SG = GetGameInstance()->GetSubsystem<USGSubSystem>();
//	//	//SG->HandleStartingNewPlayer(NewPlayer);
//
//	//	//Super::HandleStartingNewPlayer_Implementation(NewPlayer);
//
//
//	//	//// Now we're ready to override spawn location
//	//	//// Alternatively we could override core spawn location to use store locations immediately (skipping the whole 'find player start' logic)
//	//	//SG->OverrideSpawnTransform(NewPlayer);
//	//}
//	//else
//	//{
//	//	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
//	//}
//}
