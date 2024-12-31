// Copyright Epic Games, Inc. All Rights Reserved.

#include "ARPGFrameworkGameMode.h"
#include "ARPGFrameworkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AARPGFrameworkGameMode::AARPGFrameworkGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
