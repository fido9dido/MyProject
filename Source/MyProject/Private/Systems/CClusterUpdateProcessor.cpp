// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CClusterUpdateProcessor.h"
#include "Characters/Shared/CCharacterMovementComponent.h"
#include "Characters/Clusters/CNPCCharacter.h"

#include <MassStateTreeFragments.h>
#include <MassActorSubsystem.h>
#include <MassCommonTypes.h>
#include <StateTreeTypes.h>

UCClusterUpdateProcessor::UCClusterUpdateProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteAfter.Add(TEXT("MassStateTreeProcessor"));
	bRequiresGameThreadExecution = true;
}

void UCClusterUpdateProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassStateTreeInstanceFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<UMassStateTreeSubsystem>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FMassStateTreeSharedFragment>();
}

void UCClusterUpdateProcessor::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	//TODO handle state from AI when it's implement in C++

	EntityQuery.ForEachEntityChunk(entityManager, context, [this](FMassExecutionContext& context)
		{
			if (context.GetConstSharedFragmentPtr<FMassStateTreeSharedFragment>() == nullptr) { return; }

			const TConstArrayView<FMassStateTreeInstanceFragment> stateTreeInstanceList = context.GetFragmentView<FMassStateTreeInstanceFragment>();
			const TArrayView<FMassActorFragment> actorFragments = context.GetMutableFragmentView<FMassActorFragment>();

			const int32 numEntities = context.GetNumEntities();

			UMassStateTreeSubsystem& massStateTreeSubsystem = context.GetMutableSubsystemChecked<UMassStateTreeSubsystem>();

			for (int32 i = 0; i < numEntities; ++i)
			{
				const FMassStateTreeSharedFragment& sharedStateTree = context.GetConstSharedFragment<FMassStateTreeSharedFragment>();
				FStateTreeInstanceData* stateTreeInstanceData = massStateTreeSubsystem.GetInstanceData(stateTreeInstanceList[i].InstanceHandle);
				const FStateTreeExecutionState* executionState = stateTreeInstanceData->GetExecutionState();
				if (!executionState || executionState->ActiveStates.IsEmpty()) { continue; }


				const FStateTreeStateHandle& stateHandle = executionState->ActiveStates.Last();
				const FCompactStateTreeState* stateData = sharedStateTree.StateTree->GetStateFromHandle(stateHandle);

				AActor* actor = actorFragments[i].GetMutable();
				if (!actor) { continue; }
				ACNPCCharacter* character = Cast<ACNPCCharacter>(actor);
				UCCharacterMovementComponent* movementComponent = Cast<UCCharacterMovementComponent>(character->GetMovementComponent());

				if (stateData)
				{
					if (stateData->Name.IsEqual(FName(TEXT("Idle"))))
					{
						movementComponent->Velocity = FVector::Zero();

						//TODO handle state from AI when it's implement in C++
						movementComponent->CurrentLocomotionState = ECLocomotionState::Idle;
					}
					else if (stateData->Name.IsEqual(FName(TEXT("Jog"))))
					{
						movementComponent->CurrentLocomotionState = ECLocomotionState::Jog;
					}
				}
			}
		});
			
}

