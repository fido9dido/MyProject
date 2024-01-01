// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/FindResourceTask.h"
#include "Fragments/WorkerFragment.h"
#include "Fragments/SmartObjectQueryResultFragment.h"

#include "MassStateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "MassEntitySubsystem.h"
#include "MassAIBehaviorTypes.h"
#include "SmartObjectComponent.h"
#include "MassActorSubsystem.h"
#include "MassSmartObjectFragments.h"


FFindResourceTask::FFindResourceTask()
{		  	
	bShouldStateChangeOnReselect = false;
}

bool FFindResourceTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(WorkerFragmentHandle);
	Linker.LinkExternalData(SmartObjectUserFragmentHandle);
	Linker.LinkExternalData(MassEntitySubsystemHandle);
	Linker.LinkExternalData(SmartObjectSubsystemHandle);
	Linker.LinkExternalData(MassActorSubsystemHandle);
	return true;
}

EStateTreeRunStatus FFindResourceTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	FWorkerFragment& workerFragment = Context.GetExternalData(WorkerFragmentHandle);
	FMassSmartObjectUserFragment& smartObjectUserFragment = Context.GetExternalData(SmartObjectUserFragmentHandle);
	UMassEntitySubsystem& massEntitySubsystem = Context.GetExternalData(MassEntitySubsystemHandle);
	FMassEntityManager& entityManager = massEntitySubsystem.GetMutableEntityManager();

	FMassEntityHandle& assignedStructure = workerFragment.GetWorkplaceHandle();
	
	if (!assignedStructure.IsValid() || workerFragment.JobClaimHandle.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	FSmartObjectQueryResultFragment* smartObjectQueryResultFragment = entityManager.GetFragmentDataPtr<FSmartObjectQueryResultFragment>(assignedStructure);

	while (!smartObjectQueryResultFragment->SortedCandidateSlots.IsEmpty())
	{
		FSmartObjectCandidateSlot smartObjectRequestResult = smartObjectQueryResultFragment->SortedCandidateSlots.Pop();
		if (smartObjectRequestResult.Result.IsValid())
		{ 
			instanceData.FoundCandidateSlot = smartObjectRequestResult;
			return EStateTreeRunStatus::Running;
		}
	}

	return EStateTreeRunStatus::Failed; 

}
