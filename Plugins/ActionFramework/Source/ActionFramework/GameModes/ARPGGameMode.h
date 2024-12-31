// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

/**
 * 
 */
//class UStartMenu;
//class UDiedWidget;
//class ULoadingScreen;

UCLASS()
class ACTIONFRAMEWORK_API AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AARPGGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent , Category="ARPGGameMode")
	void StartNewMainGame();
	void StartNewMainGame_Implementation();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void LoadMainGame();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void FinishedShaderCompile();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void OnPlayerDead();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void LoadStartMenu();

	UFUNCTION(BlueprintCallable, Category = "ARPGGameMode")
	void OnBossDeath();
protected:

	UPROPERTY(EditDefaultsOnly, Category = "ARPGGameMode")
	TSubclassOf<UUserWidget> StartGameMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "ARPGGameMode")
	TSubclassOf<UUserWidget> EndGameFadeInWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "ARPGGameMode")
	FName MainGameLevelName;

	/*TObjectPtr<UStartMenu> StartGameMenuWidget;
	*/
	UPROPERTY(EditDefaultsOnly, Category = "ARPGGameMode")
	TObjectPtr<UUserWidget> EndGameFadeInWidget;
	UPROPERTY(EditDefaultsOnly, Category = "ARPGGameMode")
	FName StartGameLevelName;

	//TObjectPtr<UDiedWidget> DiedWidget;
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UUserWidget> DiedWidgetClass;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UUserWidget> LoadingWidgetClass;
	//UPROPERTY(BlueprintReadWrite)
	//TObjectPtr<ULoadingScreen> LoadingWidget;


	bool bIsDataLoad;
private:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	bool bIsGameStart;
};
