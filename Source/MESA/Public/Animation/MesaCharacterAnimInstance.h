// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/MesaCharacterBase.h"
#include "Character/Misc/MesaCharacterStructLibrary.h"
#include "Character/Movement/MesaCharacterStates.h"
#include "MesaCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MESA_API UMesaCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanSprinting() const;

protected:
	void UpdateAnimationProperties();

	void UpdateMovementValues(float DeltaTime);

	void CalculateMovementDirection();

	FDirectionBlending CalculateDirectionBlending() const;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AMesaCharacterBase> PossessedCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimSettings AnimSettings;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimDirection MovementDirection = ECharacterMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FDirectionBlending DirectionBlending;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ECharacterMovementState MovementState = ECharacterMovementState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ECharacterGait CharacterGait = ECharacterGait::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ECharacterStance CharacterStance = ECharacterStance::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementOffset = 0.f;
};
