// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Misc/MesaCharacterStructLibrary.h"
#include "Movement/MesaCharacterStates.h"
#include "MesaCharacterBase.generated.h"

struct FMovementSettings;
class UDebugComponent;
class UMesaCharacterMovementComponent;

UCLASS()
class MESA_API AMesaCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMesaCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void Landed(const FHitResult & Hit) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FORCEINLINE UMesaCharacterMovementComponent* GetMesaMovementComponent() const { return CharacterMovementComponent; }

private:
	// Character states
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Movement")
	ECharacterMovementState MovementState = ECharacterMovementState::Grounded;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Movement")
	ECharacterGait CharacterGait = ECharacterGait::Running;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Movement")
	ECharacterStance CharacterStance = ECharacterStance::Idle;

public:
	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FORCEINLINE ECharacterMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	void SetMovementState(const ECharacterMovementState NewState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FORCEINLINE ECharacterGait GetCharacterGait() const { return CharacterGait; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	void SetCharacterGait(const ECharacterGait NewGait);

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FORCEINLINE ECharacterStance GetCharacterStance() const { return CharacterStance; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	void SetCharacterStance(const ECharacterStance NewGait);

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	void InitMovementSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FORCEINLINE FMovementStateSettings GetMovementStateSettings() const { return MovementStateSettings; }

	// Server functions [MOVEMENT]
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Settings|Movement")
	void Server_SetCharacterGait(const ECharacterGait NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Settings|Movement")
	void Server_SetCharacterStance(const ECharacterStance NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Settings|Movement")
	void Server_SetCharacterMovementState(const ECharacterMovementState NewGait);

	// Movement
	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	float GetSpeed() const { return CharacterSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FVector GetCharacterVelocity() const { return Velocity; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FVector GetReplicatedAcceleration() const { return ReplicatedCurrentAcceleration; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Movement")
	FRotator GetReplicatedControlRotation() const { return ReplicatedControlRotation; };

public:
	// Camera
	UFUNCTION(BlueprintCallable, Category = Camera)
	FVector GetFPCameraLocation() const;

	UFUNCTION(BlueprintCallable, Category = Camera)
	FVector GetDebugCameraLocation() const;

private:
	void SetBasicMovementValues(float DeltaTime);

	void UpdateMovement();

	void UpdateCharacterRotation(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;

public:
	// Character input actions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input")
	void MoveAction(const FVector2D& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input")
	void LookAction(const FVector2D& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input")
	void SprintAction(const bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input")
	void JumpAction(const bool bValue);

protected:
	UPROPERTY(EditAnywhere)
	float LookUpDownRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDebugComponent> DebugComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> TPMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> FPMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArmComponent = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	UDebugComponent* GetDebugComponent() const { return DebugComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMesaCharacterMovementComponent> CharacterMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Movement")
	FDataTableRowHandle CharacterSettings;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	FMovementStateSettings MovementStateSettings;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	float CharacterSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	FVector PreviousVelocity = FVector::ZeroVector;

	// Character rotation
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Settings|Rotation")
	FVector ReplicatedCurrentAcceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Settings|Rotation")
	FRotator ReplicatedControlRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Rotation")
	float MaxAcceleration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Rotation")
	FRotator LastVelocityRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Rotation")
	FRotator LastMovementInputRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Movement")
	float MovementInputAmount = 0.0f;
};