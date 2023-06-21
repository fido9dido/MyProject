// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/NavMeshPathFollowTask.h"
#include <MassStateTreeExecutionContext.h>
#include <NavigationSystem.h>
#include <StateTreeLinker.h>

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
	// Tick doesn't seem to be called when we enter State, so we call it manually
	return Tick(context, 0.f);
}

EStateTreeRunStatus FNavMeshPathFollowTask::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
	FMassStateTreeExecutionContext& massContext = static_cast<FMassStateTreeExecutionContext&>(context);
	const FAgentRadiusFragment& agentRadius = context.GetExternalData(AgentRadiusHandle);
	FMassMoveTargetFragment& moveTarget = context.GetExternalData(MoveTargetHandle);
	FMoveTaskInstanceData& instanceData = context.GetInstanceData<FMoveTaskInstanceData>(*this);
	FNavMeshFragment& navMeshFragment = context.GetExternalData(NavMeshFragmentHandle);

	const FMassMovementParameters& movementParams = context.GetExternalData(MovementParamsHandle);

	const FVector agentNavLocation = context.GetExternalData(TransformHandle).GetTransform().GetLocation();
	UNavigationSystemV1* navMeshSubsystem = Cast<UNavigationSystemV1>(context.GetWorld()->GetNavigationSystem());
	

	const FNavAgentProperties& navAgentProperties = FNavAgentProperties(agentRadius.Radius);
	if (const ANavigationData* navigationData = navMeshSubsystem->GetNavDataForProps(navAgentProperties, agentNavLocation))
	{
		FPathFindingQuery query(navMeshSubsystem, *navigationData, agentNavLocation, instanceData.TargetLocation);

		if (query.NavData.IsValid() == false)
		{
			query.NavData = navMeshSubsystem->GetNavDataForProps(navAgentProperties, query.StartLocation);
		}

		FPathFindingResult result(ENavigationQueryResult::Error);
		if (query.NavData.IsValid())
		{
			result = query.NavData->FindPath(navAgentProperties, query);
		}

		FNavigationPath* navigationPath = result.Path.Get();;
		if (result.IsSuccessful() && navigationPath->GetPathPoints().Num() > 1)
		{
		
			const FVector endLocation = navigationPath->GetEndLocation();

			moveTarget.DistanceToGoal = (endLocation - agentNavLocation).Length();
			if (moveTarget.DistanceToGoal > agentRadius.Radius)
			{
				const float desiredSpeed = FMath::Min(
					movementParams.GenerateDesiredSpeed(instanceData.MovementStyle, massContext.GetEntity().Index) * instanceData.SpeedScale ,
					movementParams.MaxSpeed);

				moveTarget.DesiredSpeed.Set(desiredSpeed);
				moveTarget.Center = navigationPath->GetEndLocation();
				moveTarget.Forward = (endLocation - agentNavLocation).GetSafeNormal();
				moveTarget.CreateNewAction(EMassMovementAction::Move, *context.GetWorld());

				const TArray<FNavPathPoint>& pathPoints = navigationPath->GetPathPoints();

				navMeshFragment.NextPathNodePos = pathPoints[1].Location;
			} 
			else 
			{
				moveTarget.DesiredSpeed.Set(0.f);	 
				moveTarget.DistanceToGoal = 0.f;
			}
			
			if (moveTarget.DistanceToGoal < agentRadius.Radius)
			{
				return EStateTreeRunStatus::Succeeded;
			}

#if WITH_EDITOR
			navigationPath->DebugDraw(query.NavData.Get(), FColor::MakeRandomColor(), context.GetWorld()->GetCanvasForRenderingToTarget(), false, 10.0f);
#endif
		}
		else
		{
			return EStateTreeRunStatus::Failed;
		}
	}
	return EStateTreeRunStatus::Running;
}
