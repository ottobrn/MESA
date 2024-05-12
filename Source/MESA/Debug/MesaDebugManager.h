// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MesaDebugManager.generated.h"

USTRUCT()
struct FGameDebugTextInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString Header = FString();

	UPROPERTY()
	FString Value = FString();

	UPROPERTY()
	FString MenuName = FString();

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

	UFUNCTION()
	void SetCharacter(class AMesaCharacterBase* NewCharacter);

	UFUNCTION(BlueprintCallable)
	ECharacterGait GetCharacterGait() const;

	UFUNCTION(BlueprintCallable)
	ECharacterStance GetCharacterStance() const;

	UFUNCTION(BlueprintCallable)
	ECharacterMovementDirection GetCharacterMovementDirection() const;

	UFUNCTION(BlueprintCallable)
	ECharacterMovementState GetCharacterMovementState() const;
	
	void EmplaceDebugInfo(const FString& InHeader, const FGameDebugTextInfo& InInfo);

	const TMap<FString, TArray<FGameDebugTextInfo>>& GetDebugInfo() { return DebugInfo; }
	
private:
	UPROPERTY()
	TObjectPtr<AMesaCharacterBase> CharacterInstance = nullptr;

	TMap<FString, TArray<FGameDebugTextInfo>> DebugInfo;
};
