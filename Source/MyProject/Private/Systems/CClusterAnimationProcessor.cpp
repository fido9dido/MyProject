// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CClusterAnimationProcessor.h"
#include "Characters/Shared/CCharacterMovementComponent.h"
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
	//TODO handle state from AI when it's implement in C++

	EntityQuery.ForEachEntityChunk(entityManager, context, [this](FMassExecutionContext& context)
		{

			const TArrayView<FMassActorFragment> actorFragments = context.GetMutableFragmentView<FMassActorFragment>();

			const int32 numEntities = context.GetNumEntities();

			for (int32 i = 0; i < numEntities; ++i)
			{
				AActor* actor = actorFragments[i].GetMutable();
				if (!actor) { continue; }
				ACNPCCharacter* character = Cast<ACNPCCharacter>(actor);
				UCCharacterMovementComponent* movementComponent = Cast<UCCharacterMovementComponent>(character->GetMovementComponent());

				// Tick Animation manually for now
				USkeletalMeshComponent* skeletalMeshComponent = character->GetSkeletalMeshComponent();
				if (UWorld* world = character->GetWorld())
				{
					skeletalMeshComponent->TickAnimation(world->DeltaTimeSeconds, false);
					skeletalMeshComponent->RefreshBoneTransforms();
				}
			}
		});
			
}

