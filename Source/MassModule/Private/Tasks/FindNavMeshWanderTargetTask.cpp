// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/FindNavMeshWanderTargetTask.h"
#include <MassStateTreeExecutionContext.h>
#include <NavigationSystem.h>
#include <StateTreeLinker.h>
#include <MassAIBehaviorTypes.h>

bool FFindNavMeshWanderTargetTask::Link(FStateTreeLinker& linker)
{
	linker.LinkExternalData(PointOfInterestsSharedHandle);
	return true;
};

EStateTreeRunStatus FFindNavMeshWanderTargetTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UNavigationSystemV1* navSystem = Cast<UNavigationSystemV1>(Context.GetWorld()->GetNavigationSystem());
	
	if (ensure(navSystem != nullptr))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	FPointoOfInterestsSharedFragment& pointOfInterests = Context.GetExternalData(PointOfInterestsSharedHandle);
	const FVector& origin = pointOfInterests.GetLocation();
	const float radius = pointOfInterests.GetRadius();
	
	FNavLocation navLocation;
	navSystem->GetRandomReachablePointInRadius(origin, radius, navLocation);

	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	instanceData.TargetLocation = navLocation.Location;

	return EStateTreeRunStatus::Running;
};

