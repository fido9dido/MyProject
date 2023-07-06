// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <GlobalTypes.h>
#include "CClusterAnimationInstance.generated.h"


UCLASS()
class UCClusterAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient, BlueprintReadWrite, Category = "References")
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "References")
	TObjectPtr<class UCCharacterMovementComponent> MovementComponent;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	FVector Velocity;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	float Speed;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	float MaxSpeed;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	float WalkSpeedThreshold;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	float Height;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	bool bIsFalling = false;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Essential Data")
	FRotator CharacterRotation;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	ECLocomotionState LocomotionState;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	float TimeInLocomotionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Data")
	float TimeInLocomotionStateThreshold;

public:

	UCClusterAnimationInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativePostEvaluateAnimation() override;

	void SetLocomotionState(const ECLocomotionState locomotionState);

	UFUNCTION(BlueprintPure, Category = "Locomotion")
	void GetMoveState(EMassMovementAction& OutCurrentMovementAction, EMassMovementAction& OutPreviousMovementAction) const;

};

