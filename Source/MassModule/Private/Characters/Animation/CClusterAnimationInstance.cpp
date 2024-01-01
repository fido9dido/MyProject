// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Animation/CClusterAnimationInstance.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <MassNavigationTypes.h>
#include "GlobalTypes.h"
#include <Characters/CCharacterMovementComponent.h>

UCClusterAnimationInstance::UCClusterAnimationInstance(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{

	WalkSpeedThreshold = 1.f;
	TimeInLocomotionStateThreshold = 0.15f;
}

void UCClusterAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	QUICK_SCOPE_CYCLE_COUNTER(STAT_UCClusterAnimationInstance_NativeInitializeAnimation);

	// Set References and DefaultValues
	Character = Cast<ACharacter>(TryGetPawnOwner());
	
	if (Character)
	{
		MovementComponent = Cast<UCCharacterMovementComponent>(Character->GetCharacterMovement());
	}
}

void UCClusterAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	QUICK_SCOPE_CYCLE_COUNTER(STAT_UCClusterAnimationInstance_NativeUpdateAnimation);

	if(Character && MovementComponent)
	{
		// Set Essential Data

		Velocity = MovementComponent->Velocity;

		Speed = Velocity.Size2D();

		MaxSpeed = MovementComponent->GetMaxSpeed();
		  
		bIsFalling = MovementComponent->IsFalling();

		CharacterRotation = Character->GetActorRotation();


		ECLocomotionState LastLocomotionState = LocomotionState;

		if (Character->HasAnyRootMotion())
		{
			LocomotionState = ECLocomotionState::Idle;
		}
		else
		{
			TimeInLocomotionState += DeltaSeconds;

			if (TimeInLocomotionState > TimeInLocomotionStateThreshold)
			{
				// Check movement data to determine desired locomotion state.
				if (bIsFalling)
				{
					LocomotionState = ECLocomotionState::Jump;
				}
				else if (Speed > WalkSpeedThreshold)
				{
					LocomotionState = ECLocomotionState::Jog;
				}
				else
				{
					LocomotionState = ECLocomotionState::Idle;
				}
			}
		}

		if ((LastLocomotionState != ECLocomotionState::Idle && LocomotionState == ECLocomotionState::Idle) ||
			(LastLocomotionState != ECLocomotionState::Jog && LocomotionState == ECLocomotionState::Jog))
		{
			TimeInLocomotionState = 0.f;
		}
	}
}

void UCClusterAnimationInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
	// Leaving this here to find crowd's post evals easily in perf captures
	QUICK_SCOPE_CYCLE_COUNTER(STAT_UCClusterAnimationInstance_NativePostEvaluateAnimation);
}

void UCClusterAnimationInstance::SetLocomotionState(const ECLocomotionState locomotionState)
{
	LocomotionState = locomotionState;
}

void UCClusterAnimationInstance::GetMoveState(EMassMovementAction& OutCurrentMovementAction, EMassMovementAction& OutPreviousMovementAction) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_UCClusterAnimationInstance_GetMassMoveState);

	OutCurrentMovementAction = EMassMovementAction::Stand;
	OutPreviousMovementAction = EMassMovementAction::Stand;

	if(Character && Character->IsPlayerControlled())
	{
		if(Speed > WalkSpeedThreshold)
		{
			OutCurrentMovementAction = EMassMovementAction::Move;
			OutPreviousMovementAction = EMassMovementAction::Move;
		}
	}
}