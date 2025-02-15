// Fill out your copyright notice in the Description page of Project Settings.

#include "MesaCharacterBase.h"
#include "MesaCharacterMovementComponent.h"
#include "MesaPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MESA/MESA.h"
#include "Kismet/KismetMathLibrary.h"
#include "MESA/Camera/MesaPlayerCameraManager.h"
#include "MESA/Debug/DebugComponent.h"
#include "MESA/Debug/MesaDebugHelpers.h"
#include "MESA/Debug/MesaDebugManager.h"
#include "MESA/Libraries/MesaCharacterStatesLibrary.h"
#include "MESA/Libraries/MesaCharacterStructLibrary.h"
#include "Net/UnrealNetwork.h"

AMesaCharacterBase::AMesaCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UMesaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	DebugComponent = CreateDefaultSubobject<UDebugComponent>(TEXT("DebugComponent"));
	LookUpDownRate = 45.f;

	TPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPMesh"));
	TPMesh->SetupAttachment(RootComponent);
}

void AMesaCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitMovementSettings();

	if (!CharacterMovementComponent->IsFalling())
	{
		SetMovementState(ECharacterMovementState::Grounded);
	}
}

void AMesaCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterMovementComponent = Cast<UMesaCharacterMovementComponent>(Super::GetMovementComponent());
}

void AMesaCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Movement replication vars
	DOREPLIFETIME_CONDITION(AMesaCharacterBase, MovementState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMesaCharacterBase, CharacterGait, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AMesaCharacterBase, ReplicatedCurrentAcceleration, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMesaCharacterBase, ReplicatedControlRotation, COND_SkipOwner);
}

void AMesaCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetMesaMovementComponent() == nullptr)
	{
		return;
	}
	
	switch (GetMesaMovementComponent()->MovementMode)
	{
	case EMovementMode::MOVE_Walking:
		SetMovementState(ECharacterMovementState::Grounded);
		break;
	case EMovementMode::MOVE_NavWalking:
		SetMovementState(ECharacterMovementState::Grounded);
		break;
	case EMovementMode::MOVE_Falling:
		SetMovementState(ECharacterMovementState::InAir);
		break;
	case EMovementMode::MOVE_Swimming:
		SetMovementState(ECharacterMovementState::Swimming);
		break;
	default:
		SetMovementState(ECharacterMovementState::None);
		break;
	}
}

void AMesaCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void AMesaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetBasicMovementValues(DeltaTime);

	if (MovementState == ECharacterMovementState::Grounded)
	{
		UpdateMovement();
		UpdateCharacterRotation(DeltaTime);
	}
}

FVector AMesaCharacterBase::GetFPCameraLocation() const
{
	return TPMesh->GetSocketLocation(FName("SOCKET_Camera"));
}

bool AMesaCharacterBase::IsFirstPersonCamera() const
{
	AMesaPlayerCameraManager* CameraManager = Cast<AMesaPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0));
	if (CameraManager)
	{
		return !CameraManager->IsFPCameraActive();
	}
	// Since TP camera is only in debug mode
	return true;
}

void AMesaCharacterBase::UpdateCharacterRotation(float DeltaTime)
{
	if (IsFirstPersonCamera())
	{
		if (GetLocalRole() != ROLE_SimulatedProxy)
		{
			ReplicatedControlRotation = GetControlRotation();
		}
		SmoothRotation = UKismetMathLibrary::RInterpTo(SmoothRotation, GetControlRotation(), DeltaTime, 30);
	}
}

void AMesaCharacterBase::SetBasicMovementValues(float DeltaTime)
{
	Velocity = GetVelocity();

	const FVector NewAcceleration = (Velocity - PreviousVelocity) / DeltaTime;
	Acceleration = NewAcceleration.IsNearlyZero() || IsLocallyControlled() ? NewAcceleration : Acceleration / 2;

	CharacterSpeed = GetVelocity().Size2D();
	bIsMoving = CharacterSpeed > 0 ? true : false;

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		MaxAcceleration = GetCharacterMovement()->GetMaxAcceleration() != 0 ? GetCharacterMovement()->GetMaxAcceleration() : MaxAcceleration / 2;
	}
	else
	{
		ReplicatedCurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
		MaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();
	}

	if (bIsMoving)
	{
		LastVelocityRotation = Velocity.ToOrientationRotator();
	}

	MovementInputAmount = ReplicatedCurrentAcceleration.Size() / MaxAcceleration;
	if (MovementInputAmount > 0.0f)
	{
		LastMovementInputRotation = ReplicatedCurrentAcceleration.ToOrientationRotator();
	}

	PreviousVelocity = Velocity;
}

void AMesaCharacterBase::UpdateMovement()
{
	const float RunningSpeed = GetMovementStateSettings().MovementSettings.RunMaxSpeed;
	if (MovementState == ECharacterMovementState::Grounded)
	{
		SetCharacterGait(CharacterSpeed > RunningSpeed ? ECharacterGait::Sprinting : ECharacterGait::Running);
	}
}

bool AMesaCharacterBase::CanSprint() const
{
	if (!bIsMoving && CharacterStance == ECharacterStance::Idle)
	{
		return false;
	}

	if (CharacterGait != ECharacterGait::Sprinting && MovementState == ECharacterMovementState::Grounded)
	{
		return true;
	}
	return false;
}

void AMesaCharacterBase::SetMovementState(const ECharacterMovementState NewState)
{
	if (NewState != MovementState)
	{
		MovementState = NewState;
		if (IsLocallyControlled() && !HasAuthority())
		{
			Server_SetCharacterMovementState(NewState);
		}
		ADD_DEBUG_INFO(AnimHeaderName, "Movement State", EnumToString(MovementState), MovementStatsMenuName);
	}
}

void AMesaCharacterBase::SetCharacterGait(const ECharacterGait NewGait)
{
	if (NewGait != CharacterGait)
	{
		CharacterGait = NewGait;
		if (IsLocallyControlled() && !HasAuthority())
		{
			Server_SetCharacterGait(NewGait);
		}
		ADD_DEBUG_INFO(AnimHeaderName, "Character Gait", EnumToString(CharacterGait), MovementStatsMenuName);
	}
}

void AMesaCharacterBase::SetCharacterStance(const ECharacterStance NewStance)
{
	if (NewStance != CharacterStance)
	{
		CharacterStance = NewStance;
		if (!HasAuthority())
		{
			Server_SetCharacterStance(NewStance);
		}
		ADD_DEBUG_INFO(AnimHeaderName, "Character Stance", EnumToString(CharacterStance), MovementStatsMenuName);
	}
}

void AMesaCharacterBase::InitMovementSettings()
{
	const FString ContextString = GetFullName();
	const FMovementStateSettings* FoundRow = CharacterSettings.DataTable->FindRow<FMovementStateSettings>(CharacterSettings.RowName, ContextString);

	if (FoundRow)
	{
		MovementStateSettings = *FoundRow;
	}
}

void AMesaCharacterBase::Server_SetCharacterGait_Implementation(const ECharacterGait NewGait)
{
	SetCharacterGait(NewGait);
}


void AMesaCharacterBase::Server_SetCharacterStance_Implementation(const ECharacterStance NewStance)
{
	SetCharacterStance(NewStance);
}

void AMesaCharacterBase::Server_SetCharacterMovementState_Implementation(const ECharacterMovementState NewState)
{
	SetMovementState(NewState);
}

void AMesaCharacterBase::MoveAction_Implementation(const FVector2D& Value)
{
	const FVector& ForwardVector = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	AddMovementInput(ForwardVector, Value.Y);

	const FVector& RightVector = UKismetMathLibrary::GetRightVector(GetActorRotation());
	AddMovementInput(RightVector, Value.X);
}

void AMesaCharacterBase::LookAction_Implementation(const FVector2D& Value)
{
	AddControllerPitchInput(LookUpDownRate * Value.Y);
	AddControllerYawInput(LookUpDownRate * Value.X);
}

void AMesaCharacterBase::SprintAction_Implementation(const bool bValue)
{
	if (bValue && CanSprint())
	{
		SetCharacterGait(ECharacterGait::Sprinting);
	}
	else
	{
		SetCharacterGait(ECharacterGait::Running);
	}
}

void AMesaCharacterBase::JumpAction_Implementation(const bool bValue)
{
	if (bValue)
	{
		if (MovementState == ECharacterMovementState::Grounded)
		{
			if (CharacterStance == ECharacterStance::Idle)
			{
				Jump();
			}
			else if (CharacterStance == ECharacterStance::Crouch)
			{
				UnCrouch();
			}
		}
	}
	else
	{
		StopJumping();
	}
}
