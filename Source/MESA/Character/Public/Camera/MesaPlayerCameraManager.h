// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
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

public:
	UFUNCTION()
	void InitializeProperties(AMesaCharacterBase* InCharacter);
	
	UFUNCTION(BlueprintCallable)
	bool IsFPCameraActive() const;

	UFUNCTION()
	void SwitchCurrentCamera();

protected:
	UFUNCTION()
	void UpdateDebugCamera(FTViewTarget& OutVT, float DeltaTime);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMesaCharacterBase> PossessedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Debug")
	float CameraLocationOffset = 400.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFPCameraActive = true;
};
