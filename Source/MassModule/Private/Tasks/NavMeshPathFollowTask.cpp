// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/NavMeshPathFollowTask.h"
#include <MassStateTreeExecutionContext.h>
#include <NavigationSystem.h>
#include <StateTreeLinker.h>
#include "Utilities/Util.h"
#include "NavigationPath.h"
#include <AIController.h>
#include <MassAIBehaviorTypes.h>

FNavMeshPathFollowTask::FNavMeshPathFollowTask()
{
	bShouldCallTick = true;
}

bool FNavMeshPathFollowTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(TransformHandle);
	Linker.LinkExternalData(MoveTargetHandle);
	Linker.LinkExternalData(AgentRadiusHandle);
	Linker.LinkExternalData(MovementParamsHandle);
	Linker.LinkExternalData(NavMeshFragmentHandle);
	return true;
}

EStateTreeRunStatus FNavMeshPathFollowTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	EStateTreeRunStatus result = RequestNewPath(Context);

	return  result == EStateTreeRunStatus::Running ? Tick(Context, 0.f) : result;
}

EStateTreeRunStatus FNavMeshPathFollowTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FNavMeshFragment& navMeshFragment = Context.GetExternalData(NavMeshFragmentHandle);

	if (!navMeshFragment.PathPoints.IsValidIndex(navMeshFragment.NextPathNodeIndex))
	{
		navMeshFragment.ResetPathCache();
		return EStateTreeRunStatus::Succeeded;
	}

	FMassStateTreeExecutionContext& massContext = static_cast<FMassStateTreeExecutionContext&>(Context);
	FMassMoveTargetFragment& moveTarget = Context.GetExternalData(MoveTargetHandle);
	const FMassMovementParameters& movementParams = Context.GetExternalData(MovementParamsHandle);
	const FAgentRadiusFragment& agentRadius = Context.GetExternalData(AgentRadiusHandle);
	const FVector agentLocation = Context.GetExternalData(TransformHandle).GetTransform().GetLocation();
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);

	for (int32 i = navMeshFragment.NextPathNodeIndex; i < navMeshFragment.PathPoints.Num(); ++i)
	{
		const FVector& nextGoal = navMeshFragment.PathPoints[i].Location;
		// Distance to the first Goal
		FVector direction = nextGoal - agentLocation;
		moveTarget.DistanceToGoal = direction.Length();

		if (moveTarget.DistanceToGoal > navMeshFragment.Tolerance)
		{
			navMeshFragment.NextPathNodeIndex = i;
			const float desiredSpeed = FMath::Min(
				movementParams.GenerateDesiredSpeed(instanceData.MovementStyle, massContext.GetEntity().Index) * instanceData.SpeedScale,
				movementParams.MaxSpeed);

			moveTarget.DesiredSpeed.Set(desiredSpeed);
			moveTarget.Center = nextGoal;
			moveTarget.Forward = direction.GetSafeNormal();
			moveTarget.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());
			break;
		}
	}

	if (navMeshFragment.NextPathNodeIndex == -1)
	{
		moveTarget.DesiredSpeed.Set(0.f);
		moveTarget.DistanceToGoal = 0.f;
	}

	if (moveTarget.DistanceToGoal < navMeshFragment.Tolerance)
	{
		return EStateTreeRunStatus::Succeeded;
	}

#if WITH_EDITOR
	FUtil::DrawDebugPath(massContext.GetWorld(), navMeshFragment.PathPoints, FColor::MakeRandomColor(), Context.GetWorld()->GetCanvasForRenderingToTarget(), false, 10.0f);
#endif		
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FNavMeshPathFollowTask::RequestNewPath(FStateTreeExecutionContext& Context) const
{
	FNavMeshFragment& navMeshFragment = Context.GetExternalData(NavMeshFragmentHandle);
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	UNavigationSystemV1* navigationSystem = Cast<UNavigationSystemV1>(Context.GetWorld()->GetNavigationSystem());
	const FVector agentLocation = Context.GetExternalData(TransformHandle).GetTransform().GetLocation();
	const FAgentRadiusFragment& agentRadius = Context.GetExternalData(AgentRadiusHandle);
	const FNavAgentProperties& navAgentProperties = FNavAgentProperties(agentRadius.Radius);
	const ANavigationData* navigationData = navigationSystem->GetNavDataForProps(navAgentProperties, agentLocation);

	if (!ensure(navigationData)) 
	{
		return EStateTreeRunStatus::Failed;
	}

	// Request Path
	FPathFindingQuery query(navigationSystem, *navigationData, agentLocation, instanceData.TargetLocation);

	if (query.NavData.IsValid() == false)
	{
		query.NavData = navigationSystem->GetNavDataForProps(navAgentProperties, query.StartLocation);
	}

	FPathFindingResult result(ENavigationQueryResult::Error);
	if (query.NavData.IsValid())
	{
		result = query.NavData->FindPath(navAgentProperties, query);
	}

	if (!result.IsSuccessful())
	{
		return EStateTreeRunStatus::Failed;
	}

	FNavigationPath* navigationPath = result.Path.Get();

	if (navigationPath->GetPathPoints().Num() < 2) 
	{
		return EStateTreeRunStatus::Succeeded;
	}

	navMeshFragment.SetNewPathCache(navigationPath->GetPathPoints(), 1);

	return EStateTreeRunStatus::Running;
}
