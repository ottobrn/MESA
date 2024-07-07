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

	UFUNCTION()
	void ZoomCamera(float Value);

protected:
	UFUNCTION()
	void UpdateDebugCamera(FTViewTarget& OutVT, float DeltaTime);

	UFUNCTION()
	void UpdateFPCamera(FTViewTarget& OutVT, float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Debug")
	float CameraLocationOffset = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZAxisSmothingValue = 3.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFPCameraActive = true;

	UPROPERTY(BlueprintReadOnly)
	float ZoomValue = 0.f;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMesaCharacterBase> PossessedCharacter;

private:
	UPROPERTY()
	float ZAxisSmothing = 0.f;
};
