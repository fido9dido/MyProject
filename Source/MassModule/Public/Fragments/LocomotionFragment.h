// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"

#include "GlobalTypes.h"
#include "LocomotionFragment.generated.h"


USTRUCT()
struct MASSMODULE_API FLocomotionFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	//Two states for future blending
	ECLocomotionState CurrentState = ECLocomotionState::Idle;
	ECActorState CurrentActorState = ECActorState::Patrol;
};
