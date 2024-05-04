// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if ALLOW_GAMEPLAY_DEBUGGER
#include <imgui.h>
#endif

template<typename T>
FString EnumToString(const T EnumValue)
{
	return StaticEnum<T>()->GetNameStringByValue(static_cast<T>(EnumValue));
}
