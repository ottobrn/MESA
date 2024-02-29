// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Camera/MesaPlayerCameraManager.h"
#include "Debug/DebugComponent.h"

void AMesaPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (OutVT.Target)
	{
		if (OutVT.Target.IsA<AMesaCharacterBase>())
		{
			OutVT.POV.Location = PossessedCharacter->GetFPCameraLocation();
			OutVT.POV.Rotation = PossessedCharacter->GetControlRotation();
#if !UE_BUILD_SHIPPING
			PossessedCharacter->bUseControllerRotationYaw = !PossessedCharacter->GetDebugComponent()->bUseDebugCamera;
			if (PossessedCharacter->GetDebugComponent()->bUseDebugCamera)
			{
				OutVT.POV.Location = FVector(PossessedCharacter->GetDebugCameraLocation().X, PossessedCharacter->GetDebugCameraLocation().Y, 550.f);
			}
#endif
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
