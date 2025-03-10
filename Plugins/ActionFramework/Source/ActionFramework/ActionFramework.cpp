// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionFramework.h"
#include "AbilitySystem/ARPGAbilitySystemGlobals.h"
#define LOCTEXT_NAMESPACE "FActionFrameworkModule"

void FActionFrameworkModule::StartupModule()
{


    //UAbilitySystemGlobals::Get().AbilitySystemGlobalsClassName = FSoftClassPath(TEXT("/Script/ActionFramework.ARPGAbilitySystemGlobals"));

   
}

void FActionFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FActionFrameworkModule, ActionFramework)