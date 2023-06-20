// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CClusterAnimationProcessor.h"
#include "Characters/Clusters/CNPCCharacter.h"

#include <MassRepresentationTypes.h>
#include <MassActorSubsystem.h>
#include <MassCommonTypes.h>
#include <Fragments/ClusterVisualizationFragment.h>

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

void UCClusterAnimationProcessor::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	EntityQuery.ForEachEntityChunk(entityManager, context, std::bind(&UCClusterAnimationProcessor::Tick, this,std::placeholders::_1));
			
}

void UCClusterAnimationProcessor::Tick(FMassExecutionContext& context)
{
	const TArrayView<FMassActorFragment> actorFragments = context.GetMutableFragmentView<FMassActorFragment>();

	const int32 numEntities = context.GetNumEntities();

	for (int32 i = 0; i < numEntities; ++i)
	{
		AActor* actor = actorFragments[i].GetMutable();
		if (!actor) { continue; }
		ACNPCCharacter* character = Cast<ACNPCCharacter>(actor);
		
		USkeletalMeshComponent* skeletalMeshComponent = character->GetSkeletalMeshComponent();
		if (UWorld* world = character->GetWorld())
		{
			skeletalMeshComponent->TickAnimation(world->DeltaTimeSeconds, false);
			skeletalMeshComponent->RefreshBoneTransforms();
		}
	}
}

