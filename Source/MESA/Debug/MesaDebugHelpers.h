// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "MesaDebugManager.h"
#include "Kismet/KismetSystemLibrary.h"

class UKismetSystemLibrary;

// Headers
static const FString AnimHeaderName = FString("Animations");
// Menus
static const FString MovementStatsMenuName = FString("Movement Stats");
static const FString MovementValuesMenuName = FString("Movement Values");

namespace Private
{
	FORCEINLINE void AddDebugInfo(const FString& InHeader, const FString& InName, const FString& InValue, const FString& MenuName)
	{
#if ALLOW_GAMEPLAY_DEBUGGER
		if (GEngine && GEngine->GameViewport)
		{
			UMesaDebugManager* DebugManager = UMesaDebugManager::Get(GEngine->GameViewport->GetWorld());
			if (IsValid(DebugManager))
			{
				FGameDebugTextInfo NewTextInfo;
				NewTextInfo.Header = InName;
				NewTextInfo.Value = TCHAR_TO_UTF8(*InValue);
				NewTextInfo.MenuName = MenuName;

				DebugManager->EmplaceDebugInfo(InHeader, NewTextInfo);
			}
		}
#endif
	}
}

#define TRACE_WARNING(Message, Duration) \
	if (GEngine)\
	{ \
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Orange, Message); \
	} \
	
#define TRACE_ERROR(Message, Duration) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, Message); \
	} \

#define TRACE(Message, Duration)\
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::White, Message); \
	} \
	
#define ADD_DEBUG_INFO(HeaderName, InfoName, Value, MenuName)\
	Private::AddDebugInfo(HeaderName, InfoName, Value, MenuName);\
