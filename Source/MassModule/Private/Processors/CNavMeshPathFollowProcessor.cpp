// Fill out your copyright notice in the Description page of Project Settings.

#include "Processors/CNavMeshPathFollowProcessor.h"
#include <MassCommonTypes.h>
#include "Fragments/NavMeshFragment.h"
#include <MassMovementFragments.h>
#include <MassNavigationFragments.h>
#include <MassSignalSubsystem.h>
#include <MassExecutionContext.h>
#include <MassStateTreeTypes.h>
#include <functional>

UCNavMeshPathFollowProcessor::UCNavMeshPathFollowProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	bRequiresGameThreadExecution = true;
}

void UCNavMeshPathFollowProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	SignalSubsystem = UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
}

void UCNavMeshPathFollowProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FNavMeshFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassVelocityFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<UMassSignalSubsystem>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
	
	ProcessorRequirements.AddSubsystemRequirement<UMassSignalSubsystem>(EMassFragmentAccess::ReadWrite);

}

void UCNavMeshPathFollowProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCNavMeshPathFollowProcessor::Tick, this, std::placeholders::_1));

	if (EntitiesToSignalPathDone.Num() > 0)
	{
		SignalSubsystem->SignalEntities(UE::Mass::Signals::NewStateTreeTaskRequired, EntitiesToSignalPathDone);
		EntitiesToSignalPathDone.Empty();
	}
}

void UCNavMeshPathFollowProcessor::Tick(FMassExecutionContext& Context)
{
	const TArrayView<FNavMeshFragment> navMeshFragmentList = Context.GetMutableFragmentView<FNavMeshFragment>();
	const TArrayView<FTransformFragment> transformFragmentList = Context.GetMutableFragmentView<FTransformFragment>();
	const TArrayView<FMassVelocityFragment> velocityFragmentList = Context.GetMutableFragmentView<FMassVelocityFragment>();
	const TArrayView<FMassMoveTargetFragment> moveTargetFragmentList = Context.GetMutableFragmentView<FMassMoveTargetFragment>();

	for (int32 i = 0; i < Context.GetNumEntities(); ++i)
	{

		FNavMeshFragment& navMeshFragment = navMeshFragmentList[i];
		FMassVelocityFragment& velocityFragment = velocityFragmentList[i];
		FMassMoveTargetFragment& moveTargetFragment = moveTargetFragmentList[i];
		const FTransform& agentTransform = transformFragmentList[i].GetTransform();

		FVector direction = (moveTargetFragment.Center - agentTransform.GetLocation()).GetSafeNormal();
		velocityFragment.Value = direction * moveTargetFragment.DesiredSpeed.Get();
		moveTargetFragment.DistanceToGoal = (moveTargetFragment.Center - agentTransform.GetLocation()).Length();
		const float distanceToNextGoal = (moveTargetFragment.Center - agentTransform.GetLocation()).Length();

		if (distanceToNextGoal < navMeshFragment.Tolerance)
		{
			EntitiesToSignalPathDone.Add(Context.GetEntity(i));
		}
	}
}

