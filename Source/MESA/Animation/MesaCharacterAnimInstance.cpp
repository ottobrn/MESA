// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MESA/MESA.h"
#include "MESA/Character/MesaCharacterBase.h"
#include "MESA/Character/MesaCharacterMovementComponent.h"
#include "MESA/Libraries/MesaMathLibrary.h"

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

bool UMesaCharacterAnimInstance::IsGrounded() const
{
	return CharacterAnimStates.MovementState == ECharacterMovementState::Grounded;
}

void UMesaCharacterAnimInstance::CalculateMovementDirection()
{
	if (!AnimSettings.bIsMoving)
	{
		MovementOffset = 0.f;
	}

	MovementOffset = UKismetAnimationLibrary::CalculateDirection(AnimSettings.Velocity, PossessedCharacter->GetActorRotation());
	{
		const float RFThreshold = 60.f;
		const float LFThresold = -60.f;
		const float RBThresold = 110.f;
		const float LBThresold = -110.f;

		auto CalcOffset = [](ECharacterMovementDirection CurrentDirection)
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

void UMesaCharacterAnimInstance::CalculateLeanXYValue(float DeltaTime)
{
	if (AnimSettings.bIsMoving)
	{
		const FVector2D Acceleration2D = FVector2D(AnimSettings.Acceleration.X, AnimSettings.Acceleration.Y);
		const FVector2D Velocity2D = FVector2D(AnimSettings.Velocity.X, AnimSettings.Velocity.Y);
		
		float DotProduct = FVector2D::DotProduct(Acceleration2D, Velocity2D);

		const float CurrentAcceleration = DotProduct > 0.f ? GetMaxAcceleration() : GetMaxBreakingDeceleration();
		
		const FVector& ClampedAccelerationSize = AnimSettings.Acceleration.GetClampedToMaxSize(CurrentAcceleration);
		RelativeAcceleration = PossessedCharacter->GetActorRotation().UnrotateVector(ClampedAccelerationSize / CurrentAcceleration);

		LeanXY = UKismetMathLibrary::VInterpTo(LeanXY, RelativeAcceleration, DeltaTime, BlendingHelpers.LeanInterpSpeed);
	}
	else
	{
		if (LeanXY != FVector::ZeroVector)
		{
			LeanXY = UKismetMathLibrary::VInterpTo(LeanXY, FVector::ZeroVector, DeltaTime, BlendingHelpers.LeanInterpSpeed);
		}
	}
}

float UMesaCharacterAnimInstance::GetMaxAcceleration() const
{
	if (PossessedCharacter == nullptr)
	{
		return 0.f;
	}
	
	if (UMesaCharacterMovementComponent* MovementComponent = PossessedCharacter->GetMesaMovementComponent())
	{
		return MovementComponent->GetMaxAcceleration();
	}
	return 0.f;
}

float UMesaCharacterAnimInstance::GetMaxBreakingDeceleration() const
{
	if (PossessedCharacter == nullptr)
	{
		return 0.f;
	}
	
	if (UMesaCharacterMovementComponent* MovementComponent = PossessedCharacter->GetMesaMovementComponent())
	{
		return MovementComponent->GetMaxBrakingDeceleration();
	}
	return 0.f;
}

FDirectionBlending UMesaCharacterAnimInstance::CalculateDirectionBlending() const
{
	const FVector& VelocityDirection = PossessedCharacter->GetActorRotation().UnrotateVector(AnimSettings.Velocity.GetSafeNormal(0.1f));
	const float VecComponentSum = FMath::Abs(VelocityDirection.X) + FMath::Abs(VelocityDirection.Y) + FMath::Abs(VelocityDirection.Z);

	if (VecComponentSum > 0.f)
	{
		const FVector& RelativeDirection = VelocityDirection / VecComponentSum;
		FDirectionBlending LocalBlending;
		LocalBlending.F = FMath::Clamp(RelativeDirection.X, 0.f, 1.f);
		LocalBlending.B = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.f, 0.f));
		LocalBlending.R = FMath::Clamp(RelativeDirection.Y, 0.f, 1.f);
		LocalBlending.L = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.f, 0.f));

		return LocalBlending;
	}
	return FDirectionBlending();
}

void UMesaCharacterAnimInstance::UpdateAnimationProperties()
{
	CharacterAnimStates.CharacterGait = PossessedCharacter->GetCharacterGait();
	CharacterAnimStates.CharacterStance = PossessedCharacter->GetCharacterStance();
	CharacterAnimStates.MovementState = PossessedCharacter->GetMovementState();
	
	AnimSettings.Speed = PossessedCharacter->GetSpeed();
	AnimSettings.Acceleration = PossessedCharacter->GetReplicatedAcceleration();
	AnimSettings.ControlRotation = PossessedCharacter->GetSmoothedRotation();
	AnimSettings.bIsMoving = PossessedCharacter->IsMoving() && IsGrounded();
	AnimSettings.Velocity = PossessedCharacter->GetCharacterVelocity();
}

void UMesaCharacterAnimInstance::UpdateMovementValues(float DeltaTime)
{
	CalculateMovementDirection();
	CalculateLeanXYValue(DeltaTime);
	
	const FDirectionBlending& TargetBlending = CalculateDirectionBlending();
	DirectionBlending.F = FMath::FInterpTo(DirectionBlending.F, TargetBlending.F, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.B = FMath::FInterpTo(DirectionBlending.B, TargetBlending.B, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.R = FMath::FInterpTo(DirectionBlending.R, TargetBlending.R, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.L = FMath::FInterpTo(DirectionBlending.L, TargetBlending.L, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
}

void UMesaCharacterAnimInstance::UpdateRotationValues()
{
	const FRotator& Delta = (AnimSettings.Velocity.ToOrientationRotator() - AnimSettings.ControlRotation).GetNormalized();
	
	const FVector& FBOffset = YawOffset_FB->GetVectorValue(Delta.Yaw);
	FYaw = FBOffset.X;
	BYaw = FBOffset.Y;
	
	const FVector& LROffset = YawOffset_LR->GetVectorValue(Delta.Yaw);
	LYaw = LROffset.X;
	RYaw = LROffset.Y;
}

