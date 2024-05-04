// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiDelegates.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MesaUIManager.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UMesaUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UMesaUIManager* Get(UWorld* World);

protected:

public:
	void ToggleDebugMenu();

protected:
	TObjectPtr<class UMesaDebugMenu> DebugMenuWidget;

	TSharedPtr<SWidget> DebugMenu;

	FImGuiDelegate ImGuiDelegate;
	FImGuiDelegateHandle ImGuiHandle;
};
