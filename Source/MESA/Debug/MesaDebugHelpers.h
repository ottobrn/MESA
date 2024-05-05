// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

class UKismetSystemLibrary;

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