// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/ClaimSmartObjectTask.h"
#include "Fragments/WorkerFragment.h"
#include "Fragments/SmartObjectQueryResultFragment.h"

#include <SmartObjectSubsystem.h>
#include <MassSmartObjectFragments.h>
#include <MassStateTreeExecutionContext.h>
#include <StateTreeLinker.h>
#include <MassAIBehaviorTypes.h>
#include <MassEntitySubsystem.h>

#include <MassActorSubsystem.h>
#include <Processors/CNavMeshPathFollowProcessor.h>

FClaimSmartObjectTask::FClaimSmartObjectTask()
{
	// This task should not react to Enter/ExitState when the state is reselected.
	bShouldStateChangeOnReselect = false;
}

bool FClaimSmartObjectTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(SmartObjectSubsystemHandle);
	Linker.LinkExternalData(MassEntitySubsystemHandle);

	return true;
}

EStateTreeRunStatus FClaimSmartObjectTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UMassEntitySubsystem& massEntitySubsystem = Context.GetExternalData(MassEntitySubsystemHandle);
	FMassEntityManager& entityManager = massEntitySubsystem.GetMutableEntityManager();
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);

	if (!instanceData.FoundCandidateSlot.Result.IsValid()) 
	{
		return EStateTreeRunStatus::Failed; 
	}

	USmartObjectSubsystem& smartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);
	FSmartObjectClaimHandle claimHandle = smartObjectSubsystem.MarkSlotAsClaimed(instanceData.FoundCandidateSlot.Result.SlotHandle);

	if (!claimHandle.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	instanceData.ClaimHandle = claimHandle;

	return EStateTreeRunStatus::Succeeded;
};

