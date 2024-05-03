// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MESA/UI/Public/MesaUserWidget.h"
#include "UObject/UnrealTypePrivate.h"
#include "UIDebugOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UUIDebugOverlay : public UMesaUserWidget
{
	GENERATED_BODY()

public:
	bool IsDebugCameraEnabled() const { return bEnableDebugCamera; }

	bool IsMovementTracesEnabled() const { return bEnableMovementTraces; }

protected:

	UPROPERTY(BlueprintReadWrite)
	bool bEnableDebugCamera = false;

	UPROPERTY(BlueprintReadWrite)
	bool bEnableMovementTraces = false;
};
