// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaPlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "MESA/Character/MesaCharacterBase.h"
#include "MESA/Debug/MesaDebugHelpers.h"

void AMesaPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (OutVT.Target)
	{
		if (OutVT.Target.IsA<AMesaCharacterBase>())
		{
			PossessedCharacter->bUseControllerRotationYaw = bIsFPCameraActive;
			if (bIsFPCameraActive)
			{
				UpdateFPCamera(OutVT, DeltaTime);
			}
			else
			{
				UpdateDebugCamera(OutVT, DeltaTime);
			}
		}
	}
}

void AMesaPlayerCameraManager::InitializeProperties(AMesaCharacterBase* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}
	PossessedCharacter = InCharacter;
}

bool AMesaPlayerCameraManager::IsFPCameraActive() const
{
	return bIsFPCameraActive;
}

void AMesaPlayerCameraManager::SwitchCurrentCamera()
{
	bIsFPCameraActive = !bIsFPCameraActive;
}

void AMesaPlayerCameraManager::ZoomCamera(float Value)
{
	// (debug purpose only)
	// Value equals -1 or 1, so multiply it to 20 (im lazy to make it blueprintable property) to increase zoom speed
	ZoomValue = FMath::Clamp(ZoomValue + (Value * 20.f), 0.f, 300.f);
}

void AMesaPlayerCameraManager::UpdateDebugCamera(FTViewTarget& OutVT, float DeltaTime)
{
#if ALLOW_GAMEPLAY_DEBUGGER
	if (!bIsFPCameraActive)
	{
		float RotationAngleYaw = PossessedCharacter->GetControlRotation().Yaw;
		float RotationAnglePitch = PossessedCharacter->GetControlRotation().Pitch;
		
		const FVector& BackwardVector = PossessedCharacter->GetActorForwardVector() * (-1.f);
		const FVector& CharacterLocation = PossessedCharacter->GetActorLocation();

		// Calculate new camera position based on character's position and rotation
		FVector CameraLocation = (PossessedCharacter->GetActorLocation() - (BackwardVector * (CameraLocationOffset - ZoomValue)));
		const float DistanceFromCharacter = FVector::Distance(CharacterLocation, CameraLocation);
		
		// Calculate new camera position using trigonometry (assuming rotation around Z axis)
		CameraLocation.X = CharacterLocation.X - DistanceFromCharacter * FMath::Cos(FMath::DegreesToRadians(RotationAngleYaw)) * FMath::Cos(FMath::DegreesToRadians(RotationAnglePitch));
		CameraLocation.Y = CharacterLocation.Y - DistanceFromCharacter * FMath::Sin(FMath::DegreesToRadians(RotationAngleYaw)) * FMath::Cos(FMath::DegreesToRadians(RotationAnglePitch));
		CameraLocation.Z = CharacterLocation.Z + DistanceFromCharacter * FMath::Sin(FMath::DegreesToRadians(RotationAnglePitch)) * -1.f;
		
		OutVT.POV.Location = UKismetMathLibrary::VInterpTo(OutVT.POV.Location, CameraLocation, DeltaTime, 10.f);
		OutVT.POV.Rotation = (CharacterLocation - CameraLocation).Rotation();
	}
#endif
}

void AMesaPlayerCameraManager::UpdateFPCamera(FTViewTarget& OutVT, float DeltaTime)
{
	const FVector& CameraLocation = PossessedCharacter->GetFPCameraLocation();
	ZAxisSmothing = FMath::FInterpTo(ZAxisSmothing, CameraLocation.Z, DeltaTime, ZAxisSmothingValue);
	
	OutVT.POV.Location = FVector(CameraLocation.X, CameraLocation.Y, ZAxisSmothing);
	OutVT.POV.Rotation = PossessedCharacter->GetControlRotation();
}
