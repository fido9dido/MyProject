// Fill out your copyright notice in the Description page of Project Settings.

#include "Processors/CClusterAnimationProcessor.h"
#include <Fragments/ClusterVisualizationFragment.h>
#include "Characters/CAgentCharacter.h"

#include <MassRepresentationTypes.h>
#include <MassActorSubsystem.h>
#include <MassCommonTypes.h>
#include <MassExecutionContext.h>
#include <functional>

UCClusterAnimationProcessor::UCClusterAnimationProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::SyncWorldToMass);
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Representation);
	ExecutionOrder.ExecuteAfter.Add(TEXT("CClusterUpdateProcessor"));

	bRequiresGameThreadExecution = true;
}

void UCClusterAnimationProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FClusterVisualizationFragment>(EMassFragmentAccess::ReadOnly);
}

void UCClusterAnimationProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCClusterAnimationProcessor::Tick, this,std::placeholders::_1));
			
}

void UCClusterAnimationProcessor::Tick(FMassExecutionContext& Context)
{
	const TArrayView<FMassActorFragment> actorFragments = Context.GetMutableFragmentView<FMassActorFragment>();

	const int32 numEntities = Context.GetNumEntities();

	for (int32 i = 0; i < numEntities; ++i)
	{
		AActor* actor = actorFragments[i].GetMutable();
		if (!actor) { continue; }

		ACAgentCharacter* character = Cast<ACAgentCharacter>(actor);
		
		USkeletalMeshComponent* skeletalMeshComponent = character->GetSkeletalMeshComponent();
		if (UWorld* world = character->GetWorld())
		{
			skeletalMeshComponent->TickAnimation(world->DeltaTimeSeconds, false);
			skeletalMeshComponent->RefreshBoneTransforms();
		}
	}
}

