// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MESAGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API AMESAGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
