// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CNavMeshPathFollowProcessor.h"
#include <MassCommonTypes.h>
#include "Fragments/NavMeshFragment.h"
#include <MassMovementFragments.h>
#include <MassNavigationFragments.h>
#include <MassSignalSubsystem.h>
#include <MassStateTreeTypes.h>

UCNavMeshPathFollowProcessor::UCNavMeshPathFollowProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionOrder.ExecuteBefore.Add(TEXT("CClusterUpdateProcessor"));
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

void UCNavMeshPathFollowProcessor::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	
	EntityQuery.ForEachEntityChunk(entityManager, context, std::bind(&UCNavMeshPathFollowProcessor::Tick, this, std::placeholders::_1));

	if (EntitiesToSignalPathDone.Num() > 0)
	{
		SignalSubsystem->SignalEntities(UE::Mass::Signals::NewStateTreeTaskRequired, EntitiesToSignalPathDone);
		EntitiesToSignalPathDone.Empty();
	}
}

void UCNavMeshPathFollowProcessor::Tick(FMassExecutionContext& context)
{
	const TArrayView<FNavMeshFragment> navMeshFragmentList = context.GetMutableFragmentView<FNavMeshFragment>();
	const TArrayView<FTransformFragment> transformFragmentList = context.GetMutableFragmentView<FTransformFragment>();
	const TArrayView<FMassVelocityFragment> velocityFragmentList = context.GetMutableFragmentView<FMassVelocityFragment>();
	const TArrayView<FMassMoveTargetFragment> moveTargetFragmentList = context.GetMutableFragmentView<FMassMoveTargetFragment>();

	for (int32 i = 0; i < context.GetNumEntities(); ++i)
	{

		FNavMeshFragment& navMesh = navMeshFragmentList[i];
		FMassVelocityFragment& velocity = velocityFragmentList[i];
		FMassMoveTargetFragment& moveTarget = moveTargetFragmentList[i];
		const FTransform& agentTransform = transformFragmentList[i].GetTransform();
		
		FVector direction = (navMesh.NextPathNodePos - agentTransform.GetLocation()).GetSafeNormal();
		velocity.Value = direction * moveTarget.DesiredSpeed.Get();
				  
		if (moveTarget.GetCurrentAction() == EMassMovementAction::Move)
		{
			moveTarget.DistanceToGoal = (moveTarget.Center - agentTransform.GetLocation()).Length();

			const float distanceToNextGoal = (navMesh.NextPathNodePos - agentTransform.GetLocation()).Length();

			if (distanceToNextGoal < navMesh.Tolerance)
			{
				EntitiesToSignalPathDone.Add(context.GetEntity(i));
			}
		}
	}
}
