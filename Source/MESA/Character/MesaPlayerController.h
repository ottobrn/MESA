// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "MesaPlayerController.generated.h"


class AMesaCharacter;
class AMesaCharacterBase;
class UInputMappingContext;
class AMesaPlayerCameraManager;
/**
 * 
 */
UCLASS()
class MESA_API AMesaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

	virtual void BindActions(UInputMappingContext* InContext);

	virtual void Tick(float DeltaSeconds) override;

public:
	TObjectPtr<AMesaCharacterBase> GetPossessedCharacter() const { return PossessedCharacter; }

protected:
	/* Initialize enhanced input */
	void InitializePlayerInputs();

	/* Initialize player camera manager */
	void InitializePlayerCamera();

public:
	UFUNCTION()
	void MoveAction(const FInputActionValue& Value);

	UFUNCTION()
	void LookAction(const FInputActionValue& Value);

	UFUNCTION()
	void SprintAction(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void SwitchCameraAction(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleDebugMenu(const FInputActionValue& Value);

	UFUNCTION()
	void ZoomCameraAction(const FInputActionValue& Value);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMesaCharacterBase> PossessedCharacter = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMesaPlayerCameraManager> MesaCameraManager = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Mesa|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Mesa|Input")
	TObjectPtr<UInputMappingContext> DebugMappingContext = nullptr;
};
