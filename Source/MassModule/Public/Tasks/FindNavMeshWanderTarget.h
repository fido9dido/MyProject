// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include "MassStateTreeTypes.h"
#include <Fragments/PointoOfInterestsFragment.h>
#include "FindNavMeshWanderTarget.generated.h"

/**
 *	GetInstanceDataType makes an instanced version of this struct in the data asset
 */

USTRUCT()
struct MASSMODULE_API FFindNavMeshTargetInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Output)
	FVector TargetLocation = FVector::ZeroVector;

};

 // Find a random point near the point of interest
USTRUCT(meta = (DisplayName = "Find NavMesh Wander Target"))
struct MASSMODULE_API FFindNavMeshWanderTarget: public FMassStateTreeTaskBase
{
	GENERATED_BODY()

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual const UStruct* GetInstanceDataType() const override { return FFindNavMeshTargetInstanceData::StaticStruct(); };

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	TStateTreeExternalDataHandle<FPointoOfInterestsSharedFragment> PointOfInterestsSharedHandle;

};