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
