// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/MesaMathLibrary.h"

bool UMesaMathLibrary::IsAngleInRange(float CurrentAngle, float MinAngle, float MaxAngle, float Offset)
{
	if (Offset != 0)
	{
		return CurrentAngle <= (MaxAngle + Offset) && CurrentAngle >= (MinAngle - Offset);
	}
	return CurrentAngle <= MaxAngle && CurrentAngle >= MinAngle;
}

void UMesaMathLibrary::NegateVector(FVector& InVector, bool bAllowZAxis)
{
	InVector.X = InVector.X * -1.f;
	InVector.Y = InVector.Y * -1.f;
	if (bAllowZAxis)
	{
		InVector.Z = InVector.Z * -1.f;
	}
}
