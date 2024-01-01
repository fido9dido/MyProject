// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "GlobalTypes.h"
#include "SmartObjectTypes.h"
#include "MassSmartObjectRequest.h"

#include "WorkerFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FWorkplaceData
{
	GENERATED_BODY()

public:
	FSmartObjectClaimHandle ClaimedSlot;
	FMassEntityHandle StructureHandle;
};

USTRUCT()
struct MASSMODULE_API FWorkerFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	FWorkplaceData WorkplaceData;
	FSmartObjectClaimHandle JobClaimHandle;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	ECWorkerType WorkerType = ECWorkerType::Unset;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float Efficiency = 1.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float MaxOutput = 10;

public:
	float GetWorkOutput() { return Efficiency * MaxOutput; }
	FSmartObjectClaimHandle& GetWorkplaceClaimHandle() { return WorkplaceData.ClaimedSlot; }
	FMassEntityHandle& GetWorkplaceHandle() { return WorkplaceData.StructureHandle; }
	void OnJobDone(class USmartObjectSubsystem& smartObjectSubsystem);
	
	
};
