// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"

#include "PointoOfInterestsFragment.generated.h"


USTRUCT()
struct MASSMODULE_API FPointoOfInterestsSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

public:
	// this is the MassSpawner Location, visible for debugging purposes
	UPROPERTY(VisibleAnywhere)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Radius = 1000.f;
public:
	FPointoOfInterestsSharedFragment() = default;

	const FVector& GetLocation() const { return Location; }
	const float& GetRadius() const { return Radius; }
};