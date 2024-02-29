// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Character/MesaCharacterBase.h"
#include "MesaPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API AMesaPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	void InitializeProperties(AMesaCharacterBase* InCharacter);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMesaCharacterBase> PossessedCharacter;

	UPROPERTY(BlueprintReadOnly)
	FVector CameraLocation = FVector::ZeroVector;
};
