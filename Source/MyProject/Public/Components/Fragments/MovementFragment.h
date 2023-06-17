// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"

#include "MovementFragment.generated.h"

USTRUCT()
struct MYPROJECT_API FMovementFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	FVector Target;
};
