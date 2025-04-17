// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionFramework.h"
#include "AbilitySystem/ARPGAbilitySystemGlobals.h"
#define LOCTEXT_NAMESPACE "FActionFrameworkModule"

void FActionFrameworkModule::StartupModule()
{


    //FString ConfigFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("ActionFramework/Config/DefaultActionFramework.ini"));

    //if (FPaths::FileExists(ConfigFilePath))
    //{
    //    FString NormalizedConfigIniPath;
    //    NormalizedConfigIniPath = FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath);
    //    UE_LOG(LogTemp, Log, TEXT("Config file found at: %s"), *ConfigFilePath);

    //    FConfigFile ConfigFile;
    //    ConfigFile.Read(ConfigFilePath);

    //    FString MySettingValue;
    //    if (ConfigFile.GetString(TEXT("/Script/GameplayAbilities.AbilitySystemGlobals"), TEXT("AbilitySystemGlobalsClassName"), MySettingValue))
    //    {
    //        UE_LOG(LogTemp, Log, TEXT("Loaded Config Value: %s"), *MySettingValue);
    //       
    //        // AbilitySystemGlobals 인스턴스를 가져와서 설정
    //        UAbilitySystemGlobals& ASG = UAbilitySystemGlobals::Get();
    //        UE_LOG(LogTemp, Log, TEXT("AbilitySystemGlobals Before initialized class Name: %s"), *ASG.AbilitySystemGlobalsClassName.ToString());


    //        ASG.ReloadConfig(nullptr, *NormalizedConfigIniPath);

    //        UE_LOG(LogTemp, Log, TEXT("AbilitySystemGlobals initialized with new class: %s"), *ASG.AbilitySystemGlobalsClassName.ToString());

    //        UAbilitySystemGlobals& AfterASG = UAbilitySystemGlobals::Get();
    //        
    //        UE_LOG(LogTemp, Log, TEXT("AbilitySystemGlobals actual class: %s"), *AfterASG.GetClass()->GetName());

    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("Failed to find key in config file."));
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Config file not found!"));
    //}
   
}

void FActionFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FActionFrameworkModule, ActionFramework)