// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Swimming,
};

UENUM(BlueprintType)
enum class ECharacterGait : uint8
{
	None,
	Running,
	Sprinting,
};

UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	None,
	Idle,
	Crouch,
};

UENUM(BlueprintType)
enum class ECharacterMovementDirection : uint8
{
	None,
	Forward,
	Backward,
	Right,
	Left,
};

UENUM(BlueprintType)
enum class ECharacterMovementSubDirection : uint8
{
	None,
	LF,
	RF,
	BL,
	BR,
};

