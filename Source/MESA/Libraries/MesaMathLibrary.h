// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MesaMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UMesaMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static bool IsAngleInRange(float CurrentAngle, float MinAngle, float MaxAngle, float Offset);

	UFUNCTION(BlueprintCallable)
	static void NegateVector(FVector& InVector, bool bAllowZAxis = true);
};
