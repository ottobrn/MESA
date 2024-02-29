// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template<typename T>
FString EnumToString(const T EnumValue)
{
	return StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(EnumValue));
}
