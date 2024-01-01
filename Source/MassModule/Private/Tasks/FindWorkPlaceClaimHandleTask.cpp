// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/FindWorkPlaceClaimHandleTask.h"
#include "Fragments/WorkerFragment.h"
#include "Fragments/SmartObjectQueryResultFragment.h"

#include "MassStateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "MassEntitySubsystem.h"
#include "MassAIBehaviorTypes.h"
#include "SmartObjectComponent.h"
#include "MassActorSubsystem.h"


FFindWorkPlaceClaimHandleTask::FFindWorkPlaceClaimHandleTask()
{		  	
	bShouldStateChangeOnReselect = false;
	bShouldCallTick = true;
}

bool FFindWorkPlaceClaimHandleTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(WorkerFragmentHandle);
	Linker.LinkExternalData(MassEntitySubsystemHandle);

	return true;
}

EStateTreeRunStatus FFindWorkPlaceClaimHandleTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	FWorkerFragment& workerFragment = Context.GetExternalData(WorkerFragmentHandle);
	FSmartObjectClaimHandle& claimedSlot = workerFragment.GetWorkplaceClaimHandle();

	if (!claimedSlot.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	instanceData.ClaimedSlot = claimedSlot;
	return EStateTreeRunStatus::Running;
}
