// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/NavMeshPathFollowTask.h"
#include <MassStateTreeExecutionContext.h>
#include <NavigationSystem.h>
#include <StateTreeLinker.h>
#include "Globals/CUtility.h"
#include "NavigationPath.h"
#include <AIController.h>

FNavMeshPathFollowTask::FNavMeshPathFollowTask()
{
	bShouldCallTick = true;
}

bool FNavMeshPathFollowTask::Link(FStateTreeLinker& linker)
{
	linker.LinkExternalData(TransformHandle);
	linker.LinkExternalData(MoveTargetHandle);
	linker.LinkExternalData(AgentRadiusHandle);
	linker.LinkExternalData(MovementParamsHandle);
	linker.LinkExternalData(NavMeshFragmentHandle);
	return true;
}

EStateTreeRunStatus FNavMeshPathFollowTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& Transition) const
{
	EStateTreeRunStatus result = RequetNewPath(context);

	return  result == EStateTreeRunStatus::Running ? Tick(context, 0.f) : result;
}

EStateTreeRunStatus FNavMeshPathFollowTask::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
	
	return UpdateMoveTarget(context );;
}

EStateTreeRunStatus FNavMeshPathFollowTask::RequetNewPath(FStateTreeExecutionContext& context) const
{
	FNavMeshFragment& navMeshFragment = context.GetExternalData(NavMeshFragmentHandle);
	FMoveTaskInstanceData& instanceData = context.GetInstanceData<FMoveTaskInstanceData>(*this);
	UNavigationSystemV1* navigationSystem = Cast<UNavigationSystemV1>(context.GetWorld()->GetNavigationSystem());
	const FVector agentLocation = context.GetExternalData(TransformHandle).GetTransform().GetLocation();
	const FAgentRadiusFragment& agentRadius = context.GetExternalData(AgentRadiusHandle);
	const FNavAgentProperties& navAgentProperties = FNavAgentProperties(agentRadius.Radius);
	const ANavigationData* navigationData = navigationSystem->GetNavDataForProps(navAgentProperties, agentLocation);

	if (!navigationData) { return EStateTreeRunStatus::Failed; }
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
	
	FNavigationPath* navigationPath = result.Path.Get();;
	if (!result.IsSuccessful() || navigationPath->GetPathPoints().Num() < 2) { return EStateTreeRunStatus::Failed; }

	navMeshFragment.SetNewPathCache(navigationPath->GetPathPoints(), 1);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FNavMeshPathFollowTask::UpdateMoveTarget(FStateTreeExecutionContext& context) const
{
	FNavMeshFragment& navMeshFragment = context.GetExternalData(NavMeshFragmentHandle);

	if (!navMeshFragment.PathPoints.IsValidIndex(navMeshFragment.NextPathNodeIndex))
	{
		navMeshFragment.ResetPathCache();
		return EStateTreeRunStatus::Succeeded;
	}
	FMassStateTreeExecutionContext& massContext = static_cast<FMassStateTreeExecutionContext&>(context);
	FMassMoveTargetFragment& moveTarget = context.GetExternalData(MoveTargetHandle);
	const FMassMovementParameters& movementParams = context.GetExternalData(MovementParamsHandle);
	const FAgentRadiusFragment& agentRadius = context.GetExternalData(AgentRadiusHandle);
	const FVector agentLocation = context.GetExternalData(TransformHandle).GetTransform().GetLocation();
	FMoveTaskInstanceData& instanceData = context.GetInstanceData<FMoveTaskInstanceData>(*this);

	for (int32& i = navMeshFragment.NextPathNodeIndex; i < navMeshFragment.PathPoints.Num(); ++i)
	{
		const FVector& nextGoal = navMeshFragment.PathPoints[i].Location;
		// Distance to the first Goal
		FVector direction = nextGoal - agentLocation;
		moveTarget.DistanceToGoal = direction.Length();

		if (moveTarget.DistanceToGoal > agentRadius.Radius)
		{
			navMeshFragment.NextPathNodeIndex = i;
			const float desiredSpeed = FMath::Min(
				movementParams.GenerateDesiredSpeed(instanceData.MovementStyle, massContext.GetEntity().Index) * instanceData.SpeedScale,
				movementParams.MaxSpeed);

			moveTarget.DesiredSpeed.Set(desiredSpeed);
			moveTarget.Center = nextGoal;
			moveTarget.Forward = direction.GetSafeNormal();
			moveTarget.CreateNewAction(EMassMovementAction::Move, *context.GetWorld());
			break;
		}
	}

	if (navMeshFragment.NextPathNodeIndex == -1)
	{
		moveTarget.DesiredSpeed.Set(0.f);
		moveTarget.DistanceToGoal = 0.f;
	}
	
	if (moveTarget.DistanceToGoal < agentRadius.Radius)
	{
		return EStateTreeRunStatus::Succeeded;
	}

#if WITH_EDITOR
	CUtility::DrawDebugPath(massContext.GetWorld(), navMeshFragment.PathPoints, FColor::MakeRandomColor(), context.GetWorld()->GetCanvasForRenderingToTarget(), false, 10.0f);
#endif		
	return EStateTreeRunStatus::Running;
}
