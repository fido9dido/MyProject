// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/FindNavMeshWanderTarget.h"
#include <MassStateTreeExecutionContext.h>
#include <NavigationSystem.h>
#include <StateTreeLinker.h>

bool FFindNavMeshWanderTarget::Link(FStateTreeLinker& linker)
{
	linker.LinkExternalData(PointOfInterestsSharedHandle);
	return true;
};
EStateTreeRunStatus FFindNavMeshWanderTarget::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	// try to make the navSystem local static
	UNavigationSystemV1* navSystem = Cast<UNavigationSystemV1>(context.GetWorld()->GetNavigationSystem());
	check(navSystem)
	if (!navSystem) { return EStateTreeRunStatus::Failed; }
	
	FPointoOfInterestsSharedFragment& pointoOfInterests = context.GetExternalData(PointOfInterestsSharedHandle);
	const FVector& poiOrigin = pointoOfInterests.GetLocation();
	const float poiRadius = pointoOfInterests.GetRadius();
	
	FNavLocation navLocation;
	
	navSystem->GetRandomReachablePointInRadius(poiOrigin, poiRadius, navLocation);

	FFindNavMeshTargetInstanceData& instanceData = context.GetInstanceData<FFindNavMeshTargetInstanceData>(*this);
	
	instanceData.TargetLocation = navLocation.Location;

	return EStateTreeRunStatus::Running;
};

