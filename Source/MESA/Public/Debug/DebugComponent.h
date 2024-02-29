// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Debug/UIDebugOverlay.h"
#include "DebugComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MESA_API UDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDebugComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void DrawMovementDebug();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AMesaCharacterBase> Owner = nullptr;

public:
	// Debug settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowDebugMovement = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseNetDraw = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseDebugCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowMovementStats = false;
};
