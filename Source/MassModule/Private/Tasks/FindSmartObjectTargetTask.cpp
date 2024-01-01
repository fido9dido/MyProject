// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/FindSmartObjectTargetTask.h"

#include "MassCommonFragments.h"
#include <MassAIBehaviorTypes.h>
#include <MassStateTreeExecutionContext.h>
#include <StateTreeExecutionContext.h>
#include <StateTreeLinker.h>

FFindSmartObjectTargetTask::FFindSmartObjectTargetTask()
{
	// This task should not react to Enter/ExitState when the state is reselected.
	bShouldStateChangeOnReselect = false;
}

bool FFindSmartObjectTargetTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(SmartObjectSubsystemHandle);

	return true;
}

EStateTreeRunStatus FFindSmartObjectTargetTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const USmartObjectSubsystem& smartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);
	FInstanceDataType&  instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	FSmartObjectClaimHandle& smartObjectClaimHandle = instanceData.ClaimedSlot;

	if (!smartObjectClaimHandle.IsValid())
	{
		MASSBEHAVIOR_LOG(Error, TEXT("FindSmartObjectTargetTask; Invalid claim handle."));
		return EStateTreeRunStatus::Failed;
	}

	const TOptional<FTransform> transform = smartObjectSubsystem.GetSlotTransform(smartObjectClaimHandle.SlotHandle);
	
	if (!transform.IsSet())
	{
		MASSBEHAVIOR_LOG(Error, TEXT("FindSmartObjectTargetTask; Invalid target location."));
		return EStateTreeRunStatus::Failed;
	}

	instanceData.TargetLocation = transform->GetLocation();
	return EStateTreeRunStatus::Running;
}
