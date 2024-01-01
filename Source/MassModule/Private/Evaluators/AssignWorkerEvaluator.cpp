// Fill out your copyright notice in the Description page of Project Settings.

#include "Evaluators/AssignWorkerEvaluator.h"
#include "SmartObjectSubsystem.h"
#include "MassSignalSubsystem.h"  
#include "StateTreeExecutionContext.h"  
#include "StateTreeLinker.h"
#include "MassSmartObjectFragments.h"
#include "MassSmartObjectBehaviorDefinition.h"
#include "MassActorSubsystem.h"
#include "Fragments/WorkerFragment.h"
#include <MassAIBehaviorTypes.h>
#include <MassStateTreeExecutionContext.h>
#include <MassEntitySubsystem.h>
#include <SmartObjectComponent.h>
#include <Fragments/SmartObjectQueryResultFragment.h>
#include <SmartObjectSubsystem.h>

bool FAssignWorkerEvaluator::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(WorkerFragmentHandle);
	Linker.LinkExternalData(ActorFragment);
	Linker.LinkExternalData(MassActorSubsystemHandle);
	Linker.LinkExternalData(SmartObjectSubsystemHandle);

	Linker.LinkExternalData(EntityTransformHandle);
	Linker.LinkExternalData(MassEntitySubsystemHandle);
	return true;
}

void FAssignWorkerEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FWorkerFragment& workerFragment = Context.GetExternalData(WorkerFragmentHandle);
	FWorkplaceData& workplaceData = workerFragment.WorkplaceData;

	if (workplaceData.ClaimedSlot.IsValid()) { return; }

	FMassActorFragment& actorFragment = Context.GetExternalData(ActorFragment);
	if (!actorFragment.IsValid())
	{
		MASSBEHAVIOR_LOG(Error, TEXT("Invalid Actor Fragment."));
		return;
	}

	USmartObjectSubsystem& smartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);
	UMassActorSubsystem& massActorSubsystem = Context.GetExternalData(MassActorSubsystemHandle);
	FTransformFragment& transformFragment = Context.GetExternalData(EntityTransformHandle);
	UMassEntitySubsystem& massEntitySubsystem = Context.GetExternalData(MassEntitySubsystemHandle);

	FInstanceDataType& instanceData = Context.GetInstanceData<FInstanceDataType>(*this);

	float& range = instanceData.Range;

	const FTransform& transform = transformFragment.GetTransform();
	//structureResults
	TArray<struct FSmartObjectRequestResult>& structureResults = instanceData.SmartObjectResults;

	if (structureResults.IsEmpty())
	{
		FSmartObjectRequest request;

		request.Filter.ActivityRequirements = GameplayTagQuery;
		request.QueryBox = FBox::BuildAABB(transform.GetLocation(), FVector(range));

		
		const AActor* userActor = actorFragment.Get();

		smartObjectSubsystem.FindSmartObjects(request, structureResults, FConstStructView::Make(FSmartObjectActorUserData(userActor)));
	}

	FMassEntityManager& entityManager = massEntitySubsystem.GetMutableEntityManager();

	//assign player to building
	while (!structureResults.IsEmpty())
	{
		FSmartObjectRequestResult structureResult = structureResults.Pop();
		if (!structureResult.IsValid()) { continue; }
		FSmartObjectClaimHandle claimedHandle = smartObjectSubsystem.MarkSlotAsClaimed(structureResult.SlotHandle);
		USmartObjectComponent* SmartObjectComponent = smartObjectSubsystem.GetSmartObjectComponent(claimedHandle);
		AActor* smartObjectActor = SmartObjectComponent->GetOwner();
		FMassEntityHandle smartObjectEntityHandle = massActorSubsystem.GetEntityHandleFromActor(smartObjectActor);
		FSmartObjectQueryResultFragment* smartObjectQueryResultFragment = entityManager.GetFragmentDataPtr<FSmartObjectQueryResultFragment>(smartObjectEntityHandle);
		if (!smartObjectQueryResultFragment) { continue; };
		workplaceData.ClaimedSlot = claimedHandle;
		workplaceData.StructureHandle = smartObjectEntityHandle;
		
		break;
	}
	
}

