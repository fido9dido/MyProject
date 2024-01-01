// Fill out your copyright notice in the Description page of Project Settings.

#include "Processors/CWorkerProcessor.h"
#include <functional>
#include "Fragments/WorkerFragment.h"
#include <MassCommonFragments.h>
#include <MassCommonTypes.h>
#include <MassExecutionContext.h>
#include <MassSmartObjectRequest.h>

#include <SmartObjectSubsystem.h>
#include <MassActorSubsystem.h>
#include "MassGameplayExternalTraits.h"

#include <MassSmartObjectFragments.h>
#include "Fragments/SmartObjectQueryResultFragment.h"
#include "Fragments/StructureFragment.h"

#include <MassStateTreeExecutionContext.h>
#include <Tags/GatherResourceTag.h>
#include <Fragments/InventoryFragment.h>
#include <Fragments/ResourceFragment.h>
#include <Fragments/StructureFragment.h>
#include <Fragments/SmartObjectFragment.h>
#include <MassEntitySubsystem.h>
#include <SmartObjectComponent.h>
#include <Tags/DepositResourceTag.h>
#include <Tags/StructureTags.h>


UCWorkerProcessor::UCWorkerProcessor()
	: AssignWorkerToStructureQuery(*this)
	, FindNearbyResourcesQuery(*this)
	, GatherResourcesQuery(*this)
	, DepositResourcesQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Behavior);
	bRequiresGameThreadExecution = true;
}

void UCWorkerProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	SmartObjectSubsystem = UWorld::GetSubsystem<USmartObjectSubsystem>(Owner.GetWorld());
	MassActorSubsystem = UWorld::GetSubsystem<UMassActorSubsystem>(Owner.GetWorld());
	ensure(SmartObjectSubsystem);
	ensure(MassActorSubsystem);
}

void UCWorkerProcessor::ConfigureQueries()
{
	AssignWorkerToStructureQuery.AddRequirement<FWorkerFragment>(EMassFragmentAccess::ReadWrite);
	AssignWorkerToStructureQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);
	AssignWorkerToStructureQuery.AddTagRequirement<FAddWorkerToStructureTag>(EMassFragmentPresence::All);
	
	FindNearbyResourcesQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	FindNearbyResourcesQuery.AddRequirement<FSmartObjectQueryResultFragment>(EMassFragmentAccess::ReadWrite);
	FindNearbyResourcesQuery.AddRequirement<FStructureFragment>(EMassFragmentAccess::ReadWrite);
	FindNearbyResourcesQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);

	GatherResourcesQuery.AddRequirement<FWorkerFragment>(EMassFragmentAccess::ReadWrite);
	GatherResourcesQuery.AddRequirement<FInventoryFragment>(EMassFragmentAccess::ReadWrite);
	GatherResourcesQuery.AddRequirement<FMassSmartObjectUserFragment>(EMassFragmentAccess::ReadWrite);
	GatherResourcesQuery.AddTagRequirement<FGatherResourceTag>(EMassFragmentPresence::All);

	DepositResourcesQuery.AddRequirement<FWorkerFragment>(EMassFragmentAccess::ReadWrite);
	DepositResourcesQuery.AddRequirement<FInventoryFragment>(EMassFragmentAccess::ReadWrite);
	DepositResourcesQuery.AddTagRequirement<FDepositResourceTag>(EMassFragmentPresence::All);
}

void UCWorkerProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	AssignWorkerToStructureQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCWorkerProcessor::AssignWorkerToStructure, this, std::placeholders::_1));
	RemoveTagFromEntities(EntityManager, FAddWorkerToStructureTag::StaticStruct());
	//Todo: move FindNearbyResourcesQuery to Building Processor when one is made
	FindNearbyResourcesQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCWorkerProcessor::FindNearbyResources, this, std::placeholders::_1));
	
	GatherResourcesQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCWorkerProcessor::GatherResources, this, std::placeholders::_1));
	RemoveTagFromEntities(EntityManager, FGatherResourceTag::StaticStruct());

	DepositResourcesQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCWorkerProcessor::DepositResources, this, std::placeholders::_1));
	RemoveTagFromEntities(EntityManager, FDepositResourceTag::StaticStruct());
}

void UCWorkerProcessor::RemoveTagFromEntities(FMassEntityManager& EntityManager, const UScriptStruct* TagType)
{
	for (FMassEntityHandle& entity : EntitiesToRemove)
	{
		EntityManager.RemoveTagFromEntity(entity, TagType);

	}
	EntitiesToRemove.Reset();
}

void UCWorkerProcessor::AssignWorkerToStructure(FMassExecutionContext& Context)
{
	const TArrayView<FWorkerFragment> workerFragmentList = Context.GetMutableFragmentView<FWorkerFragment>();
	const TArrayView<FMassActorFragment> massActorFragmentList = Context.GetMutableFragmentView<FMassActorFragment>();

	FSmartObjectRequestFilter smartObjectRequestFilter;
	TArray<FSmartObjectSlotHandle> slotHandles;

	const int32 numEntities = Context.GetNumEntities();
	FMassEntityManager& entityManager  = Context.GetEntityManagerChecked();
	for (int32 i = 0; i < numEntities; ++i)
	{
		FWorkerFragment& workerFragment = workerFragmentList[i];
		FWorkplaceData& workplaceData = workerFragment.WorkplaceData;
		
		FSmartObjectFragment& smartObjectFragment = entityManager.GetFragmentDataChecked<FSmartObjectFragment>(workplaceData.StructureHandle);
		FStructureFragment& structureFragment = entityManager.GetFragmentDataChecked<FStructureFragment>(workplaceData.StructureHandle);
		FMassActorFragment& massActorFragment = massActorFragmentList[i];
	
		if (!ensure(smartObjectFragment.RegisteredHandle.IsValid())) { continue; }
		
		slotHandles.Reset();
		smartObjectRequestFilter.ActivityRequirements = structureFragment.StructureQuery;
			
		SmartObjectSubsystem->FindSlots(smartObjectFragment.RegisteredHandle, smartObjectRequestFilter, slotHandles, FConstStructView::Make(FSmartObjectActorUserData(massActorFragment.Get())));
		if (slotHandles.IsEmpty()) 
		{ 
			continue;
		}
		FSmartObjectSlotHandle structureResult = slotHandles.Pop();

		//The handle that contain the structure and the slot handles 
		FSmartObjectClaimHandle claimedHandle = SmartObjectSubsystem->MarkSlotAsClaimed(structureResult);
		USmartObjectComponent* SmartObjectComponent = SmartObjectSubsystem->GetSmartObjectComponent(claimedHandle);
		AActor* smartObjectActor = SmartObjectComponent->GetOwner();
		FMassEntityHandle smartObjectEntityHandle = MassActorSubsystem->GetEntityHandleFromActor(smartObjectActor);
	
		workplaceData.ClaimedSlot = claimedHandle;
		EntitiesToRemove.Emplace(Context.GetEntity(i));
	}
}

void UCWorkerProcessor::RemoveWorkerFromStructure(FMassExecutionContext& Context)
{
}

void UCWorkerProcessor::FindNearbyResources(FMassExecutionContext& Context)
{
	const TArrayView<FSmartObjectQueryResultFragment> smartObjectQueryResultList = Context.GetMutableFragmentView<FSmartObjectQueryResultFragment>();
	const TArrayView<FTransformFragment>  transformFragmentList = Context.GetMutableFragmentView<FTransformFragment>();
	const TArrayView <FStructureFragment> structureFragmentList = Context.GetMutableFragmentView<FStructureFragment>();
	const TArrayView <FMassActorFragment> massActorFragmentList = Context.GetMutableFragmentView<FMassActorFragment>();
	
	const int32 numEntities = Context.GetNumEntities();
	for (int32 i = 0; i < numEntities; ++i)
	{
		FVector buildingLocation = transformFragmentList[i].GetTransform().GetLocation();
		FMassActorFragment& actorFragment = massActorFragmentList[i];
		FStructureFragment& structureFragment = structureFragmentList[i];
		FSmartObjectQueryResultFragment& smartObjectQueryResult = smartObjectQueryResultList[i];
		smartObjectQueryResult.EntityHandle = Context.GetEntity(i);
		
		check(structureFragment.ResourceQuery.IsEmpty() == false);
			
		FSmartObjectRequest request;
		request.Filter.UserTags = structureFragment.UserTags;
		request.Filter.ActivityRequirements = structureFragment.ResourceQuery;
		request.QueryBox = FBox::BuildAABB(buildingLocation, FVector(structureFragment.BuildingRange));

		TArray<FSmartObjectRequestResult> queryResults;
		const AActor* actor = actorFragment.Get();
		const FSmartObjectActorUserData actorUserData(actor);
		const FConstStructView actorUserDataView(FConstStructView::Make(actorUserData));
		SmartObjectSubsystem->FindSmartObjects(request, queryResults, actorUserDataView);

		TArray<FSmartObjectCandidateSlot>& sortedCandidateSlots = smartObjectQueryResult.SortedCandidateSlots;

		sortedCandidateSlots.Reset(queryResults.Num());
		for (const FSmartObjectRequestResult& queryResult : queryResults)
		{
			const FVector SlotLocation = SmartObjectSubsystem->GetSlotLocation(queryResult.SlotHandle).GetValue();
			sortedCandidateSlots.Emplace(queryResult, UE_REAL_TO_FLOAT_CLAMPED_MAX(FVector::DistSquared(buildingLocation, SlotLocation)));

#if WITH_MASSGAMEPLAY_DEBUG
		
			constexpr float debugRadius = 10.f;
			FColor DebugColor(FColor::White);
			UE_VLOG_LOCATION(SmartObjectSubsystem, LogSmartObject, Display, SlotLocation, debugRadius, DebugColor, TEXT("%s"), *LexToString(queryResult.SmartObjectHandle));
			UE_VLOG_SEGMENT(SmartObjectSubsystem, LogSmartObject, Display, buildingLocation, SlotLocation, DebugColor, TEXT(""));

#endif // WITH_MASSGAMEPLAY_DEBUG
		}
		sortedCandidateSlots.Sort([](const FSmartObjectCandidateSlot& first, const FSmartObjectCandidateSlot& second) { return first.Cost < second.Cost; });
	}
}

void UCWorkerProcessor::GatherResources(FMassExecutionContext& Context)
{
	FMassEntityManager& entityManager = Context.GetEntityManagerChecked();
	const TConstArrayView <FMassSmartObjectUserFragment> smartObjectUserFragmentList = Context.GetFragmentView<FMassSmartObjectUserFragment>();
	TArrayView <FWorkerFragment> workerFragmentList = Context.GetMutableFragmentView<FWorkerFragment>();
	TArrayView <FInventoryFragment> inventoryFragmentList = Context.GetMutableFragmentView<FInventoryFragment>();

	const int32 numEntities = Context.GetNumEntities();

	for (int32 i = 0; i < numEntities; ++i)
	{
		FWorkerFragment& workerFragment = workerFragmentList[i];
		FInventoryFragment& workerinventory = inventoryFragmentList[i];
		const FMassSmartObjectUserFragment& smartObjectUserFragment = smartObjectUserFragmentList[i];

		if (!workerFragment.JobClaimHandle.IsValid()) { continue; }

		USmartObjectComponent* smartObjectComponent = SmartObjectSubsystem->GetSmartObjectComponent(workerFragment.JobClaimHandle);
		AActor* smartObjectActor = smartObjectComponent->GetOwner();
		FMassEntityHandle resourceHandle = MassActorSubsystem->GetEntityHandleFromActor(smartObjectActor);
		FResourceFragment& resourceFragment = entityManager.GetFragmentDataChecked<FResourceFragment>(resourceHandle);
		FInventoryFragment& resourceInventory = entityManager.GetFragmentDataChecked<FInventoryFragment>(resourceHandle);

		float amountToGather = workerFragment.GetWorkOutput();
		int32 gatheredResources = resourceInventory.WithdrawResource(resourceFragment.ResourceType, amountToGather);
		int32 extra = workerinventory.DepositResource(resourceFragment.ResourceType, gatheredResources);
		
		if (extra > 0) 
		{
			resourceInventory.DepositResource(resourceFragment.ResourceType, extra); 
		}
		
		if (resourceInventory.IsEmpty())
		{
			Context.Defer().DestroyEntity(resourceHandle);
		}			
		workerFragment.OnJobDone(*SmartObjectSubsystem);
		EntitiesToRemove.Emplace(Context.GetEntity(i));
	}
}

void UCWorkerProcessor::ReleaseResource(FMassEntityManager& EntityManager, FMassEntityHandle& WorkerHandle, FSmartObjectClaimHandle& ClaimHandle)
{
	EntityManager.Defer().RemoveTag<FGatherResourceTag>(WorkerHandle);

	SmartObjectSubsystem->MarkSlotAsFree(ClaimHandle);
}

void UCWorkerProcessor::DepositResources(FMassExecutionContext& Context)
{
	const TArrayView <FWorkerFragment> workerFragmentList = Context.GetMutableFragmentView<FWorkerFragment>();
	const TArrayView <FInventoryFragment> inventoryFragmentList = Context.GetMutableFragmentView<FInventoryFragment>();
	const int32 numEntities = Context.GetNumEntities();
	FMassEntityManager& entityManager = Context.GetEntityManagerChecked();
	
	for (int32 i = 0; i < numEntities; ++i)
	{
		FWorkerFragment& workerFragment = workerFragmentList[i];
		FInventoryFragment& workerinventory = inventoryFragmentList[i];
		FInventoryFragment& workplaceInventory = entityManager.GetFragmentDataChecked<FInventoryFragment>(workerFragment.GetWorkplaceHandle());

		for (TPair<ECResourceType, int32>& entry : workerinventory)
		{
			if (!workplaceInventory.IsValidType(entry.Key)) { continue; }
			 int32 extraResources = workplaceInventory.DepositResource(entry.Key, entry.Value);
			 workerinventory[entry.Key] = extraResources;
			 if (extraResources > 0) { break; }
		}

		SmartObjectSubsystem->UnMarkOccupiedSlot(workerFragment.WorkplaceData.ClaimedSlot);
		EntitiesToRemove.Emplace(Context.GetEntity(i));
	}
}
