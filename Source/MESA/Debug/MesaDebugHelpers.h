// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "MesaDebugManager.h"
#include "Kismet/KismetSystemLibrary.h"

class UKismetSystemLibrary;

// Headers
static const FName AnimHeaderName = FName("Animations");
// Menus
static const FName MovementStatsMenuName = FName("Movement Stats");
static const FName MovementValuesMenuName = FName("Movement Values");

// For internal use only
namespace Private
{
	FORCEINLINE void AddDebugInfo(const FName& InHeader, const FName& InName, const FString& InValue, const FName& MenuName)
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

	template <typename Type>
	FORCEINLINE void AddDynamicDebugInfo(const FName& InHeader, const FName& InName, Type* DynamicValue, EDebugCategory DebugCategory)
	{
#if ALLOW_GAMEPLAY_DEBUGGER
		if (GEngine && GEngine->GameViewport)
		{
			UMesaDebugManager* DebugManager = UMesaDebugManager::Get(GEngine->GameViewport->GetWorld());
			if (IsValid(DebugManager))
			{
				FGameDebugTextInfo NewTextInfo;
				NewTextInfo.Header = InName;
				NewTextInfo.DebugCategory = DebugCategory;
				NewTextInfo.DynamicValue = MakeShareable<uint8>(reinterpret_cast<uint8*>(DynamicValue));
				
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

#define ADD_DYNAMIC_DEBUG_INFO(HeaderName, InfoName, Value, DebugCategory)\
	Private::AddDynamicDebugInfo(HeaderName, InfoName, Value, DebugCategory);\
