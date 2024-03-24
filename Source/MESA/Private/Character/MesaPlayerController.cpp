// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MesaPlayerController.h"
#include "Character/MesaCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/Camera/MesaPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void AMesaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		PossessedCharacter = Cast<AMesaCharacterBase>(InPawn);
	}

	if (!IsRunningDedicatedServer())
	{
		InitializePlayerCamera();
	}

	InitializePlayerInputs();
}

void AMesaPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	// Get player pawn and cast it to our character
	PossessedCharacter = Cast<AMesaCharacterBase>(GetPawn());

	InitializePlayerInputs();
	InitializePlayerCamera();
}

void AMesaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();

		BindActions(DefaultMappingContext);
		BindActions(DebugMappingContext);
	}
}

void AMesaPlayerController::InitializePlayerInputs()
{
	if (PossessedCharacter)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (EnhancedInputLocalPlayerSubsystem)
		{
			FModifyContextOptions ModifyContextOptions;
			ModifyContextOptions.bForceImmediately = true;

			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0, ModifyContextOptions);

#if !UE_BUILD_SHIPPING
			// Dont use debug mapping context in shipping builds
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DebugMappingContext, 1, ModifyContextOptions);
#endif
		}
	}
}

void AMesaPlayerController::InitializePlayerCamera()
{
	MesaCameraManager = Cast<AMesaPlayerCameraManager>(PlayerCameraManager);
	if (MesaCameraManager)
	{
		MesaCameraManager->InitializeProperties(PossessedCharacter);
	}
}

void AMesaPlayerController::BindActions(UInputMappingContext* InContext)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	if (InContext)
	{
		const TArray<FEnhancedActionKeyMapping>& ActionMapping = InContext->GetMappings();
		// ActionMapping can store duplicate of actions. So we need to bind unique actions
		TMap<FName, TObjectPtr<const UInputAction>> UniqueActions;

		for (const FEnhancedActionKeyMapping& ActionKey : ActionMapping)
		{
			UniqueActions.Emplace(ActionKey.GetMappingName(), ActionKey.Action);
		}
		for (const TTuple<FName, TObjectPtr<const UInputAction>> CurrentAction : UniqueActions)
		{
			TObjectPtr<const UInputAction> Action = CurrentAction.Value;
			const FName& ActionName = CurrentAction.Key;

			EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, Cast<UObject>(this), ActionName);
		}
	}
}

void AMesaPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMesaPlayerController::MoveAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		const FVector2D& MoveVector = Value.Get<FVector2D>();
		PossessedCharacter->MoveAction(MoveVector);
	}
}

void AMesaPlayerController::LookAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		const FVector2D& LookAxisVector = Value.Get<FVector2D>();
		PossessedCharacter->LookAction(LookAxisVector);
	}
}

void AMesaPlayerController::SprintAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->SprintAction(Value.Get<bool>());
	}
}

void AMesaPlayerController::JumpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->JumpAction(Value.Get<bool>());
	}
}

void AMesaPlayerController::SwitchCameraAction(const FInputActionValue& Value)
{
	if (MesaCameraManager)
	{
		MesaCameraManager->SwitchCurrentCamera();
	}
}
