// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/NavMeshStandTask.h"
#include <MassStateTreeExecutionContext.h>
#include <StateTreeLinker.h>

bool FNavMeshStandTask::Link(FStateTreeLinker& linker)
{
	linker.LinkExternalData(LocationHandle);
	linker.LinkExternalData(MoveTargetHandle);
	linker.LinkExternalData(MovementParamsHandle);
	linker.LinkExternalData(MassSignalSubsystemHandle);
	return true;
}

EStateTreeRunStatus FNavMeshStandTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	const FMassStateTreeExecutionContext& massContext = static_cast<FMassStateTreeExecutionContext&>(context);
	const FMassMovementParameters& movementParameters = context.GetExternalData(MovementParamsHandle);

	FStandTaskInstanceData& instanceData = context.GetInstanceData<FStandTaskInstanceData>(*this);
	FMassMoveTargetFragment& moveTarget = context.GetExternalData(MoveTargetHandle);

	const UWorld* world = context.GetWorld();
	checkf(world != nullptr, TEXT("A valid world is expected from the execution context"));

	moveTarget.CreateNewAction(EMassMovementAction::Stand, *world);
	
	moveTarget.DistanceToGoal = 0.0f;
	moveTarget.DesiredSpeed.Set(0.0f);
	
	if (!ensureMsgf(moveTarget.GetCurrentAction() == EMassMovementAction::Stand, TEXT("Expecting action 'Stand': Invalid action %s"), moveTarget.GetCurrentAction()))
	{
		return  EStateTreeRunStatus::Failed;
	}
	moveTarget.IntentAtGoal = EMassMovementAction::Stand;
	instanceData.Time = 0.0f;

	// A Duration <= 0 indicates that the task runs until a transition in the state tree stops it.
	// Otherwise we schedule a signal to end the task.
	if (instanceData.Duration > 0.0f)
	{
		UMassSignalSubsystem& MassSignalSubsystem = context.GetExternalData(MassSignalSubsystemHandle);
		MassSignalSubsystem.DelaySignalEntity(UE::Mass::Signals::StandTaskFinished, massContext.GetEntity(), instanceData.Duration);
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FNavMeshStandTask::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
	FStandTaskInstanceData& instanceData = context.GetInstanceData<FStandTaskInstanceData>(*this);
	instanceData.Time += deltaTime;
	
	if (instanceData.Duration <= 0.0f || instanceData.Time < instanceData.Duration)
	{
		return EStateTreeRunStatus::Running;
	}
	
	return EStateTreeRunStatus::Succeeded;
	
}
