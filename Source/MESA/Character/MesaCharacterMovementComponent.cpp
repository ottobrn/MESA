// Fill out your copyright notice in the Description page of Project Settings.

#include "MesaCharacterMovementComponent.h"
#include "MesaCharacterBase.h"
#include "MESA/Libraries/MesaCharacterStructLibrary.h"

UMesaCharacterMovementComponent::UMesaCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

float UMesaCharacterMovementComponent::GetMaxSpeed() const
{
	if (MesaCharacterBase == nullptr)
	{
		return 0.f;
	}

	const FMovementSettings& MovementSettings = MesaCharacterBase->GetMovementStateSettings().MovementSettings;
	switch (MesaCharacterBase->GetCharacterGait())
	{
	case ECharacterGait::Sprinting:
		return MovementSettings.SprintMaxSpeed;
	case ECharacterGait::Running:
		return MovementSettings.RunMaxSpeed;
	default:
		return 0.0f;
	}
}

float UMesaCharacterMovementComponent::GetMaxJumpHeight() const
{
	if (MesaCharacterBase->GetMovementState() == ECharacterMovementState::InAir)
	{
		return MesaCharacterBase->GetMovementStateSettings().MovementSettings.JumpMaxHeight;
	}

	return 0.0f;
}

void UMesaCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	MesaCharacterBase = Cast<AMesaCharacterBase>(CharacterOwner);
}
