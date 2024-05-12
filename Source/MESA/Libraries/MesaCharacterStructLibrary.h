// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MesaCharacterStatesLibrary.h"
#include "MesaCharacterStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Movement)
	float RunMaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintMaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float CrouchMaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float JumpMaxHeight = 0.f;
};

USTRUCT(BlueprintType)
struct FMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Movement)
	FMovementSettings MovementSettings;
};

USTRUCT(BlueprintType)
struct FCharacterAnimSettings
{
	GENERATED_BODY();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bIsJumping = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector Acceleration = FVector::ZeroVector;;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator ControlRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Speed = 0.0f;
};

USTRUCT(BlueprintType)
struct FCharacterAnimDirection
{
	GENERATED_BODY();
	
	FCharacterAnimDirection()
	{
	}

	FCharacterAnimDirection(ECharacterMovementDirection InitDirection) : Direction(InitDirection)
	{
		bMovingForward = Direction == ECharacterMovementDirection::Forward;
		bMovingBackward = Direction == ECharacterMovementDirection::Backward;
		bMovingLeft = Direction == ECharacterMovementDirection::Left;
		bMovingRight = Direction == ECharacterMovementDirection::Right;
	}
	
	UPROPERTY(BlueprintReadOnly)
	bool bMovingForward = false;

	UPROPERTY(BlueprintReadOnly)
	bool bMovingRight= false;

	UPROPERTY(BlueprintReadOnly)
	bool bMovingLeft = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bMovingBackward = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	ECharacterMovementDirection Direction = ECharacterMovementDirection::Forward;
};

USTRUCT(BlueprintType)
struct FDirectionBlending
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly)
	float F = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float B = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float R = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float L = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DirectionLeap = 0.f;
};

USTRUCT(BlueprintType)
struct FDirectionLeap
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly)
	float F = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float B = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float R = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float L = 0.f;
};

USTRUCT(BlueprintType)
struct FBlendingHelpers
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere)
	float DirectionBlendingInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float LeanInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float RFThreshold = 70.f;

	UPROPERTY(EditAnywhere)
	float LFThresold = -70.f;

	UPROPERTY(EditAnywhere)
	float RBThresold = 110.f;

	UPROPERTY(EditAnywhere)
	float LBThresold = -110.f;
};

USTRUCT(BlueprintType)
struct FCharacterAnimStates
{
	GENERATED_BODY()

	UPROPERTY()
	ECharacterMovementState MovementState = ECharacterMovementState::Grounded;

	UPROPERTY()
	ECharacterGait CharacterGait = ECharacterGait::Running;

	UPROPERTY()
	ECharacterStance CharacterStance = ECharacterStance::Idle;
};
