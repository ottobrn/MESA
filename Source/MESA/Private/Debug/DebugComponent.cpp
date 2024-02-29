// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugComponent.h"
#include "Character/MesaCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDebugComponent::UDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AMesaCharacterBase>(GetOwner());
}

void UDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
#if !UE_BUILD_SHIPPING
	if (Owner == nullptr)
	{
		return;
	}

	//Add option in debug widget to change NetDraw
	if (bShowDebugMovement)
	{
		DrawMovementDebug();
	}
#endif
}

void UDebugComponent::DrawMovementDebug()
{
	// Draw player capsule
	const float HalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float Radius = Owner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	
	auto CalcAndDraw = [this, &Radius, &HalfHeight]() -> void
	{
		DrawDebugCapsule(GetWorld(), Owner->GetActorLocation(), HalfHeight, Radius, Owner->GetActorRotation().Quaternion(), FColor::Emerald, false, -1, 0, 1.f);

		const FVector& PlayerLocation = FVector(Owner->GetActorLocation().X, Owner->GetActorLocation().Y, Owner->GetActorLocation().Z - HalfHeight);
		DrawDebugCircle(GetWorld(), PlayerLocation, Radius * 1.5f, 100, FColor::Green, false, -1, 0, 1.5f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

		// Input direction arrow
		const FVector& RotatedVector = UKismetMathLibrary::Quat_UnrotateVector(Owner->GetActorRotation().Quaternion(), Owner->GetVelocity());
		const FVector& InputDirection = (RotatedVector.GetSafeNormal() * 50.f) + PlayerLocation;
		DrawDebugDirectionalArrow(GetWorld(), PlayerLocation, InputDirection, 50.f, FColor::Orange, false, -1.f, 0, 2.f);

		// Movement direction arrow
		const FVector& MovementDirection = (Owner->GetVelocity().GetSafeNormal() * 50.f) + PlayerLocation;
		DrawDebugDirectionalArrow(GetWorld(), PlayerLocation, MovementDirection, 50.f, FColor::Purple, false, -1.f, 0, 2.f);

		// Looking direction
		const FVector& LookingDirection = ((FRotator(0.f, Owner->GetControlRotation().Yaw, 0.f).Vector()).GetSafeNormal() * 60.f) + PlayerLocation;
		DrawDebugDirectionalArrow(GetWorld(), PlayerLocation, LookingDirection, 30.f, FColor::Yellow, false, -1.f, 0, 1.f);
	};

	// If NetDraw is false, we will draw movement debug only for the local player. True otherwise
	if (bUseNetDraw)
	{
		if (Owner->IsLocallyControlled())
		{
			CalcAndDraw();
		}
	}
	else
	{
		CalcAndDraw();
	}
}