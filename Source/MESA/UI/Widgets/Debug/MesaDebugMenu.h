// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiDelegates.h"
#include "MESA/UI/Widgets/MesaUserWidget.h"
#include "MesaDebugMenu.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UMesaDebugMenu : public UMesaUserWidget
{
	GENERATED_BODY()

public:
	void ToggleDebugMenu();

	bool IsDebugEnabled() const { return bIsDebugEnabled; }

protected:
	UFUNCTION()
	void OnDraw();

	virtual void NativeDestruct() override;

private:
	void SetupWindowStyle();
	
	void DrawAnimationSection();
	
private:
	FImGuiDelegate ImGuiDelegate;
	FImGuiDelegateHandle ImGuiHandle;

	UPROPERTY()
	bool bIsDebugEnabled = false;
	
	UPROPERTY()
	bool bIsRegistered = false;

private:
	// Instances
	UPROPERTY()
	TObjectPtr<class UMesaDebugManager> MesaDebugManager = nullptr;

	UPROPERTY()
	TObjectPtr<class AMesaCharacterBase> MesaCharacter = nullptr;

private:
	bool bIsMovementStatsOpened = false;

	bool bIsMovementValuesOpened = false;
};
