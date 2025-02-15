// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if ALLOW_GAMEPLAY_DEBUGGER
#include <imgui.h>
#endif

template<typename T>
const FString EnumToString(const T EnumValue)
{
	return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(EnumValue));
}

FORCEINLINE const char* ToCharPtr(const FString& InString)
{
	return TCHAR_TO_UTF8(*InString);
}