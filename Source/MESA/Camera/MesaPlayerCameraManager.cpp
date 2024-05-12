// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaPlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "MESA/Character/MesaCharacterBase.h"

void AMesaPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (OutVT.Target)
	{
		if (OutVT.Target.IsA<AMesaCharacterBase>())
		{
			PossessedCharacter->bUseControllerRotationYaw = bIsFPCameraActive;
			if (bIsFPCameraActive)
			{
				OutVT.POV.Location = PossessedCharacter->GetFPCameraLocation();
				OutVT.POV.Rotation = PossessedCharacter->GetControlRotation();
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


void AMesaPlayerCameraManager::UpdateDebugCamera(FTViewTarget& OutVT, float DeltaTime)
{
#if ALLOW_GAMEPLAY_DEBUGGER
	if (!bIsFPCameraActive)
	{
		float RotationAngleYaw = PossessedCharacter->GetControlRotation().Yaw;
		
		const FVector& BackwardVector = PossessedCharacter->GetActorForwardVector() * (-1.f);
		const FVector& CharacterLocation = PossessedCharacter->GetActorLocation();

		// Calculate new camera position based on character's position and rotation
		FVector CameraLocation = PossessedCharacter->GetActorLocation() - (BackwardVector * CameraLocationOffset);
		const float DistanceFromCharacter = FVector::Distance(CharacterLocation, CameraLocation);
		
		// Calculate new camera position using trigonometry (assuming rotation around Z axis)
		CameraLocation.X = CharacterLocation.X - DistanceFromCharacter * FMath::Cos(FMath::DegreesToRadians(RotationAngleYaw));
		CameraLocation.Y = CharacterLocation.Y - DistanceFromCharacter * FMath::Sin(FMath::DegreesToRadians(RotationAngleYaw));
		CameraLocation.Z = CharacterLocation.Z + 200.f;
		
		OutVT.POV.Location = UKismetMathLibrary::VInterpTo(OutVT.POV.Location, CameraLocation, DeltaTime, 10.f);
		OutVT.POV.Rotation = (CharacterLocation - CameraLocation).Rotation();
	}
#endif
}
