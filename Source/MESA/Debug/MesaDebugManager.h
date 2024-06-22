// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MesaDebugManager.generated.h"

UENUM()
enum class EDebugCategory : uint8
{
	Info,
	Checkbox,
	Slider,
};

USTRUCT()
struct FGameDebugTextInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Header = FName();

	UPROPERTY()
	FName MenuName = FName();
	
	UPROPERTY()
	FString Value = FString();

	// Use this as value that need to be changed e.g. sliders, bools etc.
	TSharedPtr<uint8> DynamicValue;

	UPROPERTY()
	EDebugCategory DebugCategory = EDebugCategory::Info;

public:
	bool operator==(const FGameDebugTextInfo& Other) const
	{
		return &Header == &Other.Header;
	}
};

/**
 * 
 */
UCLASS()
class MESA_API UMesaDebugManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// ALERT!! Always check what the Get() function returns, as its implementation is overridden by ALLOW_GAMEPLAY_DEBUGGER!
	UFUNCTION(BlueprintCallable)
	static UMesaDebugManager* Get(UWorld* World);
	
	void EmplaceDebugInfo(const FName& InHeader, const FGameDebugTextInfo& InInfo);

	const TMap<FName, TArray<FGameDebugTextInfo>>& GetDebugInfo() { return DebugInfo; }
	
private:
	UPROPERTY()
	TObjectPtr<class AMesaCharacterBase> CharacterInstance = nullptr;

	TMap<FName, TArray<FGameDebugTextInfo>> DebugInfo;
};
