// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CDefaultCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Engine/BlockingVolume.h"
#include "GameFramework/GameModeBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include <Characters/CCharacterMovementComponent.h>


ACDefaultCharacter::ACDefaultCharacter(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UCCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	PrimaryActorTick.bCanEverTick = true;
}

void ACDefaultCharacter::SetupPlayerInputComponent(class UInputComponent* playerInputComponent)
{
	// Set up gameplay key bindings
	check(playerInputComponent);

	if (UEnhancedInputComponent* const enhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent))
	{
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveActionBinding);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookActionBinding);
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &ThisClass::LookActionBinding);
		enhancedInputComponent->BindAction(LookDeltaAction, ETriggerEvent::Triggered, this, &ThisClass::LookDeltaActionBinding);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to setup player input for %s, InputComponent type is not UEnhancedInputComponent."), *GetName());
		//UE_LOG(LogCitySample, Warning, TEXT("Failed to setup player input for %s, InputComponent type is not UEnhancedInputComponent."), *GetName());
	}
}

bool ACDefaultCharacter::ShouldLimitMovementPitch() const
{
	return GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling();
}

void ACDefaultCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		const bool bLimitRotation = ShouldLimitMovementPitch();
		FRotator rotation = Controller->GetControlRotation();
		if (bLimitRotation)
		{
			rotation.Pitch = rotation.Roll = 0.0f;
		}

		// get forward vector
		const FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void ACDefaultCharacter::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		// find out which way is right
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get right vector 
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(direction, value);
	}
}

void ACDefaultCharacter::Move(const FVector2D& value)
{
	MoveForward(value.X);
	MoveRight(value.Y);
}

void ACDefaultCharacter::Look(const FVector2D& value)
{
	TurnAtRate(value.X);
	LookUpAtRate(value.Y);
}

void ACDefaultCharacter::TurnAtRate(float rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACDefaultCharacter::LookUpAtRate(float rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACDefaultCharacter::SetIsSprinting(bool bNewIsSprinting)
{
	if (bNewIsSprinting != bIsSprinting)
	{
		bIsSprinting = bNewIsSprinting;
		OnIsSprintingChanged(bNewIsSprinting);
	}
}

void ACDefaultCharacter::AddInputContext_Implementation(const TScriptInterface<IEnhancedInputSubsystemInterface>& subsystemInterface)
{
	subsystemInterface->AddMappingContext(InputMappingContext, InputMappingPriority);
	subsystemInterface->AddMappingContext(LookControlsInputMappingContext, InputMappingPriority);
}

void ACDefaultCharacter::RemoveInputContext_Implementation(const TScriptInterface<IEnhancedInputSubsystemInterface>& subsystemInterface)
{
	SetIsSprinting(false);
	subsystemInterface->RemoveMappingContext(InputMappingContext);
	subsystemInterface->RemoveMappingContext(LookControlsInputMappingContext);
}

void ACDefaultCharacter::MoveActionBinding(const struct FInputActionValue& actionValue)
{
	Move(actionValue.Get<FInputActionValue::Axis2D>());
}

void ACDefaultCharacter::LookActionBinding(const struct FInputActionValue& actionValue)
{
	Look(actionValue.Get<FInputActionValue::Axis2D>());
}

void ACDefaultCharacter::LookDeltaActionBinding(const struct FInputActionValue& actionValue)
{
	const FInputActionValue::Axis2D axisValue = actionValue.Get<FInputActionValue::Axis2D>();
	APawn::AddControllerYawInput(axisValue.X);
	APawn::AddControllerPitchInput(axisValue.Y);
}

