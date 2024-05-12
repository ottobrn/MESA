// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MESA/MESA.h"
#include "MESA/Character/MesaCharacterBase.h"
#include "MESA/Character/MesaCharacterMovementComponent.h"
#include "MESA/Debug/MesaDebugHelpers.h"
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
	PreviousMovementDirection = MovementDirection;
	if (!AnimSettings.bIsMoving)
	{
		MovementOffset = 0.f;
		MovementDirection = ECharacterMovementDirection::None;
		return;
	}

	MovementOffset = UKismetAnimationLibrary::CalculateDirection(AnimSettings.Velocity, PossessedCharacter->GetActorRotation());
	{
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
		
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, BlendingHelpers.LFThresold, BlendingHelpers.RFThreshold, CalcOffset(MovementDirection.Direction)))
		{
			MovementDirection = ECharacterMovementDirection::Forward;
			return;
		}
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, BlendingHelpers.RFThreshold, BlendingHelpers.RBThresold, CalcOffset(MovementDirection.Direction)))
		{
			MovementDirection = ECharacterMovementDirection::Right;
			return;
		}
		if (UMesaMathLibrary::IsAngleInRange(MovementOffset, BlendingHelpers.LBThresold, BlendingHelpers.LFThresold, CalcOffset(MovementDirection.Direction)))
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

void UMesaCharacterAnimInstance::CalculateDirectionLeap(float DeltaTime)
{
	if (PreviousMovementDirection.Direction != MovementDirection.Direction)
	{
		DirectionLeapTime = 0.f;
	}

	DirectionLeapTime += DeltaTime;
	DirectionBlending.DirectionLeap = DirectionLeapCurve->GetFloatValue(DirectionLeapTime);

	ADD_DEBUG_INFO(AnimHeaderName, "Direction Leap", FString::SanitizeFloat(DirectionBlending.DirectionLeap), MovementValuesMenuName);
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

void UMesaCharacterAnimInstance::PrintDebugInfo()
{
	ADD_DEBUG_INFO(AnimHeaderName, "Speed", FString::SanitizeFloat(AnimSettings.Speed), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "Acceleration", AnimSettings.Acceleration.ToString(), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "Control Rotation", AnimSettings.ControlRotation.ToString(), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "Is Moving", AnimSettings.bIsMoving ? FString("True") : FString("False"), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "Velocity", AnimSettings.Velocity.ToString(), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "DirectionBlending.F", FString::SanitizeFloat(DirectionBlending.F), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "DirectionBlending.B", FString::SanitizeFloat(DirectionBlending.B), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "DirectionBlending.R", FString::SanitizeFloat(DirectionBlending.R), MovementValuesMenuName);
	ADD_DEBUG_INFO(AnimHeaderName, "DirectionBlending.L", FString::SanitizeFloat(DirectionBlending.L), MovementValuesMenuName);
	
	ADD_DEBUG_INFO(AnimHeaderName, "Movement Direction", EnumToString(MovementDirection.Direction), MovementStatsMenuName);
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
	
	// --- Direction blending ---
	const FDirectionBlending& TargetBlending = CalculateDirectionBlending();
	DirectionBlending.F = FMath::FInterpTo(DirectionBlending.F, TargetBlending.F, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.B = FMath::FInterpTo(DirectionBlending.B, TargetBlending.B, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.R = FMath::FInterpTo(DirectionBlending.R, TargetBlending.R, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	DirectionBlending.L = FMath::FInterpTo(DirectionBlending.L, TargetBlending.L, DeltaTime, BlendingHelpers.DirectionBlendingInterpSpeed);
	CalculateDirectionLeap(DeltaTime);
	// --- Direction blending ---
	
	PrintDebugInfo();
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

