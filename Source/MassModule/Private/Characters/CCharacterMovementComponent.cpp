// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Animation/CClusterAnimationInstance.h"
#include <DrawDebugHelpers.h>

FVector UCCharacterMovementComponent::ConstrainInputAcceleration(const FVector& inputAcceleration) const
{
	FVector result = inputAcceleration;
	const float inputAccelerationSize = inputAcceleration.Size();
	if (inputAccelerationSize > SMALL_NUMBER)
	{
		const float ScaledInputAccelerationSize = FMath::Lerp(inputAccelerationSize, 1.0f, inputAccelerationSize);
		result = inputAcceleration * ScaledInputAccelerationSize / inputAccelerationSize;
	}

	result = Super::ConstrainInputAcceleration(result);
	return result;
}

float UCCharacterMovementComponent::SlideAlongSurface(const FVector& delta, float time, const FVector& inNormal, FHitResult& hit, bool bHandleImpact)
{
	FVector normal(inNormal);

	const bool bHitCharacter = hit.GetHitObjectHandle().DoesRepresentClass(ACharacter::StaticClass());
	bLastSurfaceWasCharacter = bHitCharacter;
	const float effectiveMinHorizontalSurfaceSlideAngle = bHitCharacter ? MinHorizontalSurfaceSlideAngleCharacter : MinHorizontalSurfaceSlideAngle;
	if (effectiveMinHorizontalSurfaceSlideAngle > 0.0f)
	{
		// Use input acceleration as our movement direction because our "movement delta" may already be affected by the surface slide.
		const FVector movementDirection = Acceleration.GetSafeNormal2D();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		FColor hitNormalColor = FColor::Green;
#endif

		const float movementDotNormal = movementDirection | normal;
		if (FMath::Abs(movementDotNormal) > FMath::Cos(FMath::DegreesToRadians(effectiveMinHorizontalSurfaceSlideAngle)))
		{
			// If the angle is too sharp, consider it opposite to our movement
			normal = -movementDirection;
			TimeLastSlideAlongSurfaceBlock = GetWorld()->GetTimeSeconds();
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			hitNormalColor = FColor::Red;
#endif
		}
	}

	return Super::SlideAlongSurface(delta, time, normal, hit, bHandleImpact);
}

bool UCCharacterMovementComponent::WasSlideAlongSurfaceBlockedRecently(float tolerance /*= 0.01f*/) const
{
	if (const UWorld* const world = GetWorld())
	{
		const float TimeThreshold = FMath::Max(tolerance, world->DeltaTimeSeconds + KINDA_SMALL_NUMBER);
		return world->TimeSince(TimeLastSlideAlongSurfaceBlock) <= TimeThreshold;
	}
	return false;
}