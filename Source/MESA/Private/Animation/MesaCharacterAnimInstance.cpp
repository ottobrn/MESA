// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MesaCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/MesaMathLibrary.h"
#include "MESA/MESA.h"

void UMesaCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PossessedCharacter = Cast<AMesaCharacterBase>(TryGetPawnOwner());
}

void UMesaCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UMesaCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PossessedCharacter == nullptr)
	{
		return;
	}
	
	UpdateAnimationProperties();
	UpdateMovementValues(DeltaSeconds);
}

bool UMesaCharacterAnimInstance::CanSprinting() const
{
	return AnimSettings.bIsMoving && !AnimSettings.bIsJumping;
}

void UMesaCharacterAnimInstance::CalculateMovementDirection()
{
	if (!AnimSettings.bIsMoving)
	{
		MovementOffset = 0.f;
	}

	MovementOffset = UKismetAnimationLibrary::CalculateDirection(AnimSettings.Velocity, PossessedCharacter->GetActorRotation());
	{
		const float RFThreshold = 70.f;
		const float LFThresold = -70.f;
		const float RBThresold = 110.f;
		const float LBThresold = -110.f;

		auto CalcOffset = [this](ECharacterMovementDirection CurrentDirection)
		{
			if (CurrentDirection != ECharacterMovementDirection::Forward && CurrentDirection != ECharacterMovementDirection::Backward)
			{
				return 5.f;
			}
			if (CurrentDirection != ECharacterMovementDirection::Right && CurrentDirection != ECharacterMovementDirection::Left)
			{
				return 5.f;
			}
			return 0.f;
		};
		
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, LFThresold, RFThreshold, CalcOffset(MovementDirection.Direction)))
		{
			MovementDirection = ECharacterMovementDirection::Forward;
			return;
		}
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, RFThreshold, RBThresold, CalcOffset(MovementDirection.Direction)))
		{
			MovementDirection = ECharacterMovementDirection::Right;
			return;
		}
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, LBThresold, LFThresold, CalcOffset(MovementDirection.Direction)))
		{
			MovementDirection = ECharacterMovementDirection::Left;
			return;
		}
		MovementDirection = ECharacterMovementDirection::Backward;
	}
}

FDirectionBlending UMesaCharacterAnimInstance::CalculateDirectionBlending() const
{
	const FVector& VelocityDirection = PossessedCharacter->GetActorRotation().UnrotateVector(AnimSettings.Velocity.GetSafeNormal(0.1f));
	const float VecComponentSum = FMath::Abs(VelocityDirection.X) + FMath::Abs(VelocityDirection.Y) + FMath::Abs(VelocityDirection.Z);
	if (VecComponentSum <= 0)
	{
		return FDirectionBlending();
	}

	const FVector& RelativeDirection = VelocityDirection / VecComponentSum;
	FDirectionBlending LocalBlending;
	
	LocalBlending.F = FMath::Clamp(RelativeDirection.X, 0.f, 1.f);
	LocalBlending.B = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.f, 0.f));
	LocalBlending.R = FMath::Clamp(RelativeDirection.Y, 0.f, 1.f);
	LocalBlending.L = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.f, 0.f));

	return LocalBlending;
}

void UMesaCharacterAnimInstance::UpdateAnimationProperties()
{
	AnimSettings.Speed = PossessedCharacter->GetSpeed();
	AnimSettings.Acceleration = PossessedCharacter->GetReplicatedAcceleration();
	AnimSettings.ControlRotation = PossessedCharacter->GetReplicatedControlRotation();
	AnimSettings.bIsMoving = PossessedCharacter->IsMoving();
	AnimSettings.Velocity = PossessedCharacter->GetCharacterVelocity();
}

void UMesaCharacterAnimInstance::UpdateMovementValues(float DeltaTime)
{
	CalculateMovementDirection();
	
	const FDirectionBlending& TargetBlending = CalculateDirectionBlending();
	DirectionBlending.F = FMath::FInterpTo(DirectionBlending.F, TargetBlending.F, DeltaTime, 10.f);
	DirectionBlending.B = FMath::FInterpTo(DirectionBlending.B, TargetBlending.B, DeltaTime, 10.f);
	DirectionBlending.R = FMath::FInterpTo(DirectionBlending.R, TargetBlending.R, DeltaTime, 10.f);
	DirectionBlending.L = FMath::FInterpTo(DirectionBlending.L, TargetBlending.L, DeltaTime, 10.f);
}
