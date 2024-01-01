// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/NavMeshStandTask.h"
#include <MassStateTreeExecutionContext.h>
#include <StateTreeLinker.h>
#include <MassAIBehaviorTypes.h>

bool FNavMeshStandTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(LocationHandle);
	Linker.LinkExternalData(MoveTargetHandle);
	Linker.LinkExternalData(MovementParamsHandle);
	Linker.LinkExternalData(MassSignalSubsystemHandle);
	return true;
}

EStateTreeRunStatus FNavMeshStandTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FMassStateTreeExecutionContext& massContext = static_cast<FMassStateTreeExecutionContext&>(Context);
	const FMassMovementParameters& movementParameters = Context.GetExternalData(MovementParamsHandle);

	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	FMassMoveTargetFragment& moveTarget = Context.GetExternalData(MoveTargetHandle);

	const UWorld* world = Context.GetWorld();

	ensureMsgf(world != nullptr, TEXT("FNavMeshStandTask: A valid world is expected from the execution context"));

	moveTarget.CreateNewAction(EMassMovementAction::Stand, *world);
	moveTarget.DistanceToGoal = 0.0f;
	moveTarget.DesiredSpeed.Set(0.0f);
	
	if (!ensureMsgf(moveTarget.GetCurrentAction() == EMassMovementAction::Stand, TEXT("FNavMeshStandTask: Expecting action 'Stand': Invalid action %s"), *UEnum::GetValueAsString(moveTarget.GetCurrentAction())))
	{
		return  EStateTreeRunStatus::Failed;
	}
	moveTarget.IntentAtGoal = EMassMovementAction::Stand;
	instanceData.Time = 0.0f;

	// A Duration <= 0 indicates that the task runs until a transition in the state tree stops it.
	// Otherwise we schedule a signal to end the task.
	if (instanceData.Duration > 0.0f)
	{
		UMassSignalSubsystem& MassSignalSubsystem = Context.GetExternalData(MassSignalSubsystemHandle);
		MassSignalSubsystem.DelaySignalEntity(UE::Mass::Signals::StandTaskFinished, massContext.GetEntity(), instanceData.Duration);
	}
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FNavMeshStandTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	instanceData.Time += DeltaTime;
	
	if (instanceData.Duration <= 0.0f || instanceData.Time < instanceData.Duration)
	{
		return EStateTreeRunStatus::Running;
	}
	return EStateTreeRunStatus::Succeeded;
	
}
