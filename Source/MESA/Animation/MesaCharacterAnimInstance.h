// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MESA/Libraries/MesaCharacterStatesLibrary.h"
#include "MESA/Libraries/MesaCharacterStructLibrary.h"
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsGrounded() const;

protected:
	void UpdateAnimationProperties();

	void UpdateMovementValues(float DeltaTime);
	
	void UpdateRotationValues();

	void CalculateMovementDirection();

	void CalculateLeanXYValue(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable)
	float GetMaxAcceleration() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxBreakingDeceleration() const;

	FDirectionBlending CalculateDirectionBlending() const;

	const FCharacterAnimDirection& GetMovementDirection() { return MovementDirection; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class AMesaCharacterBase> PossessedCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimSettings AnimSettings;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimDirection MovementDirection = ECharacterMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FDirectionBlending DirectionBlending;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helpers|Blending")
	FBlendingHelpers BlendingHelpers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FCharacterAnimStates CharacterAnimStates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementOffset = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector LeanXY = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector RelativeAcceleration = FVector::ZeroVector;

	// TODO: In struct
	UPROPERTY(BlueprintReadOnly)
	float FYaw = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float BYaw = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float RYaw = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float LYaw = 0.f;

	// Curves
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCurveVector> YawOffset_FB = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCurveVector> YawOffset_LR = nullptr;
};
