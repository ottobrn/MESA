// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MesaCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UMesaCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UMesaCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

public:
	virtual float GetMaxSpeed() const override;

	virtual float GetMaxJumpHeight() const override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<class AMesaCharacterBase> MesaCharacterBase = nullptr;
};