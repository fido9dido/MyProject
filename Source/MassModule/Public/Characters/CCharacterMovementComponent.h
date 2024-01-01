// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	// ConstraintInputAcceleration will interpolate the input size to go from this value to 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinInputAccelerationSize = 0.0f;

	// Minimum angle for movement to slide against. This prevents sliding against very sharp angles that will result in very small movements. Only active if greater than 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinHorizontalSurfaceSlideAngle = 0.0f;

	// Minimum angle for movement to slide against when colliding against a character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinHorizontalSurfaceSlideAngleCharacter = 0.0f;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bLastSurfaceWasCharacter = false;

private:
	float TimeLastSlideAlongSurfaceBlock = -1000.0f;

public:
	UFUNCTION(BlueprintPure)
	bool WasSlideAlongSurfaceBlockedRecently(float tolerance = 0.01f) const;

protected:
	virtual FVector ConstrainInputAcceleration(const FVector& inputAcceleration) const override;
	virtual float SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact) override;

};