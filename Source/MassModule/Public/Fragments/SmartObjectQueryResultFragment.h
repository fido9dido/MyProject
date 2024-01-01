// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassSmartObjectRequest.h"
#include "SmartObjectQueryResultFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FSmartObjectQueryResultFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	FMassEntityHandle EntityHandle;

	TArray<FSmartObjectCandidateSlot> SortedCandidateSlots;
};

