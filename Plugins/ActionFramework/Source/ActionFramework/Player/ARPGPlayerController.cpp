// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerController.h"
#include "GameFramework/Character.h"
#include "ActionFramework/Player/ARPGPlayerState.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "ActionFramework/AbilitySystem/ARPGAbilitySystemComponent.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "ActionFramework/UI/ARPGHUD.h"
#include "ActionFramework/Camerra/ARPGSpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "ActionFramework/ARPGGameplayTags.h"
//#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/Input/ARPGInputComponent.h"
#include "ActionFramework/ActionFramework.h"
#include "Kismet/KismetSystemLibrary.h"



AARPGPlayerController::AARPGPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

AARPGPlayerState* AARPGPlayerController::GetARPGPlayerState() const
{
	return CastChecked<AARPGPlayerState>(PlayerState);
}

UARPGAbilitySystemComponent* AARPGPlayerController::GetARPGAbilitySystemComponent() const
{
	const AARPGPlayerState* ARPGPS = GetARPGPlayerState();
	return (ARPGPS ? ARPGPS->GetARPGAbilitySystemComponent() : nullptr);
}

void AARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputMapping)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	//DefaultMouseCursor =  EMouseCursor::Default;

	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//SetInputMode(InputModeData);

}

void AARPGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		SpringArmComponent = Cast<UARPGSpringArmComponent>(InPawn->GetComponentByClass(UARPGSpringArmComponent::StaticClass()));
	}
}

void AARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UARPGInputComponent* InputComp = CastChecked<UARPGInputComponent>(InputComponent);

	InputComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	InputComp->BindNativeAction(InputConfig, ARPGGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move,false);
	InputComp->BindNativeAction(InputConfig, ARPGGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Look, false);
	InputComp->BindNativeAction(InputConfig, ARPGGameplayTags::Input_Esc, ETriggerEvent::Started, this, &ThisClass::ToggleEscWidget, false);
}

void AARPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetARPGAbilitySystemComponent())
	{
		GetARPGAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
	}
}

void AARPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(GetARPGAbilitySystemComponent())
		GetARPGAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void AARPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	if (GetARPGAbilitySystemComponent())
		GetARPGAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

void AARPGPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
	
}

void AARPGPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	if (GetCharacter() != nullptr)
	{

		if (!SpringArmComponent->IsCameraLockedToTarget())
		{
			GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);
		}

		if (FMath::Abs(LookAxisVector.X) < .1f)
			bAnalogSettledSinceLastTargetSwitch = true;

		if (SpringArmComponent->IsCameraLockedToTarget() && (FMath::Abs(LookAxisVector.X) > TargetSwitchAnalogValue) && bAnalogSettledSinceLastTargetSwitch)
		{

			if (LookAxisVector.X < 0)
				SpringArmComponent->SwitchTarget(ECameraDirection::Left);
			else
				SpringArmComponent->SwitchTarget(ECameraDirection::Right);

			bAnalogSettledSinceLastTargetSwitch = false;
		}
		else
		{
			// calculate delta for this frame from the rate information
			GetCharacter()->AddControllerYawInput(LookAxisVector.X);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCharacter nullptr"));
	}
}

void AARPGPlayerController::TargetLock(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("CameraLock Input"));
	SpringArmComponent->ToggleSoftLock();
}

void AARPGPlayerController::ToggleEscWidget(const FInputActionValue& Value)
{
	AARPGHUD* Arpghud = GetHUD<AARPGHUD>();
	if (Arpghud)
	{
		//AARPGHUD*  = Cast<AARPGHUD>(Hud);

		if (!Arpghud->EscWidget)
		{	
			Arpghud->InitMenuWidget(this, GetPlayerState<APlayerState>(), ASC, nullptr);
		}
		Arpghud->ToggleMenuWidget();
	}
}


void AARPGPlayerController::ChangeNextWeapon(const FInputActionValue& Value)
{
	if (UInventoryComponent* InventoryComp = GetCharacter()->GetComponentByClass<UInventoryComponent>())
	{
		InventoryComp->ChangeNextWeapon(Value.GetMagnitude());
	}
}

void AARPGPlayerController::ChangeNextTool(const FInputActionValue& Value)
{
}

void AARPGPlayerController::HandleMenuButtonClicked(FGameplayTag ButtonTag)
{
	if (ButtonTag.MatchesTag(ARPGGameplayTags::GameplayEvent_OpenInventory))
	{
		// Inventory 위젯 토글 로직
		APawn* ControlledPawn = GetPawn(); // PlayerCharacter에 대한 더 명확한 접근
		if (ControlledPawn)
		{
			//UInventoryComponent* InventoryComponent = ControlledPawn->FindComponentByClass<UInventoryComponent>();
			//if (InventoryComponent)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("InvenComp toggle InvenWidget"));
			//	//InventoryComponent->ToggleInventoryWidget();
			//	ToggleEscWidget(FInputActionValue());
			//	SetShowMouseCursor(true);

			//}
		}
	}
	else if (ButtonTag.MatchesTag(ARPGGameplayTags::GameplayEvent_OpenEquipment))
	{	
		// Inventory 위젯 토글 로직
		APawn* ControlledPawn = GetPawn(); // PlayerCharacter에 대한 더 명확한 접근
		if (ControlledPawn)
		{
			//UInventoryComponent* InventoryComponent = ControlledPawn->FindComponentByClass<UInventoryComponent>();
			//if (InventoryComponent)
			//{
			//	//InventoryComponent->ToggleEquipmentWidget();
			//	ToggleEscWidget(FInputActionValue());
			//	SetShowMouseCursor(true);
			//}
		}
	}
	else if (ButtonTag.MatchesTag(ARPGGameplayTags::GameplayEvent_ExitGame))
	{
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	}
}
