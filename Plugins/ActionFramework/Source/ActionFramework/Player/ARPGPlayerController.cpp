// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "ActionFramework/UI/EscMenuWidget.h"
#include "ActionFramework/Camerra/ARPGSpringArmComponent.h"
#include "ActionFramework/Inventory/InventoryComponent.h"
#include "ActionFramework/ActionFramework.h"
#include "Kismet/KismetSystemLibrary.h"



AARPGPlayerController::AARPGPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

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
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AARPGPlayerController::Move);
	EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARPGPlayerController::Look);
	EnhancedInputComp->BindAction(TargetLockAction, ETriggerEvent::Started, this, &AARPGPlayerController::TargetLock);
	EnhancedInputComp->BindAction(ToggleEscAction, ETriggerEvent::Started, this, &AARPGPlayerController::ToggleEscWidget);
	EnhancedInputComp->BindAction(LightAttackAction, ETriggerEvent::Started, this, &AARPGPlayerController::LightAttack);
	EnhancedInputComp->BindAction(BlockAction, ETriggerEvent::Started, this, &AARPGPlayerController::Block);
	EnhancedInputComp->BindAction(BlockAction, ETriggerEvent::Completed, this, &AARPGPlayerController::Block);

	FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/ActionFramework"), FName("EARPGAbilityInputID"));
	APlayerState* PS = GetPlayerState<APlayerState>();
	ASC = Cast<IAbilitySystemInterface>(PS)->GetAbilitySystemComponent();
	ASC->BindAbilityActivationToInputComponent(EnhancedInputComp, FGameplayAbilityInputBinds
	(
		FString("Confirm"),
		FString("Cancel"),
		AbilityEnumAssetPath,
		static_cast<int32>(EARPGAbilityInputID::Confirm),
		static_cast<int32>(EARPGAbilityInputID::Cancel)
	));

}

void AARPGPlayerController::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{

	if (!ASC)
		return;

	if (Value.Get<bool>())
	{
		ASC->AbilityLocalInputPressed(InputID);
	}
	else
	{
		ASC->AbilityLocalInputReleased(InputID);
	}
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
	if (!EscMenuWidget && EscMenuWidgetClass)
	{
		EscMenuWidget = CreateWidget<UEscMenuWidget>(GetWorld(), EscMenuWidgetClass);

		// Delegate 바인딩
		EscMenuWidget->OnMenuButtonClicked.AddDynamic(this, &AARPGPlayerController::HandleMenuButtonClicked);

		// 처음 생성 시 AddToViewport 호출
		EscMenuWidget->AddToViewport();
		EscMenuWidget->SetVisibility(ESlateVisibility::Collapsed);  // 초기 상태는 숨김
	}

	if (EscMenuWidget)
	{
		if (EscMenuWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			SetShowMouseCursor(false);
			EscMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SetShowMouseCursor(true);
			EscMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AARPGPlayerController::LightAttack(const FInputActionValue& Value)
{
	/*UE_LOG(LogTemp, Warning, TEXT("IA_LightAttack c++"));
	;
	if (!)
	{
		SendAbilityLocalInput(Value, static_cast<int32>(EARPGAbilityInputID::LightAttack));
	}*/
	FGameplayTagContainer AttackTagContainer;
	AttackTagContainer.AddTag(FGameplayTag::RequestGameplayTag("Ability.LightAttack"));
	SendAbilityLocalInput(Value, static_cast<int32>(EARPGAbilityInputID::LightAttack));

}

void AARPGPlayerController::Block(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EARPGAbilityInputID::Block));
}

void AARPGPlayerController::HandleMenuButtonClicked(FGameplayTag ButtonTag)
{
	if (ButtonTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.OepnInventory"))))
	{
		// Inventory 위젯 토글 로직
		APawn* ControlledPawn = GetPawn(); // PlayerCharacter에 대한 더 명확한 접근
		if (ControlledPawn)
		{
			UInventoryComponent* InventoryComponent = ControlledPawn->FindComponentByClass<UInventoryComponent>();
			if (InventoryComponent)
			{
				UE_LOG(LogTemp, Log, TEXT("InvenComp toggle InvenWidget"));
				InventoryComponent->ToggleInventoryWidget();
				ToggleEscWidget(FInputActionValue());
				SetShowMouseCursor(true);

			}
		}
	}
	else if (ButtonTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.OpenEquipmentWidget"))))
	{	
		// Inventory 위젯 토글 로직
		APawn* ControlledPawn = GetPawn(); // PlayerCharacter에 대한 더 명확한 접근
		if (ControlledPawn)
		{
			UInventoryComponent* InventoryComponent = ControlledPawn->FindComponentByClass<UInventoryComponent>();
			if (InventoryComponent)
			{
				InventoryComponent->ToggleEquipmentWidget();
				ToggleEscWidget(FInputActionValue());
				SetShowMouseCursor(true);
			}
		}
	}
	else if (ButtonTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("EscMenuEvent.ExitGame"))))
	{
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	}
}
