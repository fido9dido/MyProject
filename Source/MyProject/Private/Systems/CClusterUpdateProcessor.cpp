// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CClusterUpdateProcessor.h"
#include "Characters/Shared/CCharacterMovementComponent.h"
#include "Characters/Clusters/CNPCCharacter.h"

#include <MassStateTreeFragments.h>
#include <MassActorSubsystem.h>
#include <MassCommonTypes.h>
#include <StateTreeTypes.h>
#include <MassMovementFragments.h>
#include <Animation/CClusterAnimationInstance.h>
#include <Fragments/LocomotionFragment.h>

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
	EntityQuery.AddRequirement<FLocomotionFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassVelocityFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddSubsystemRequirement<UMassStateTreeSubsystem>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FMassStateTreeSharedFragment>();
}

void UCClusterUpdateProcessor::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	EntityQuery.ForEachEntityChunk(entityManager, context, std::bind(&UCClusterUpdateProcessor::Tick, this, std::placeholders::_1));
}

void UCClusterUpdateProcessor::Tick(FMassExecutionContext& context)
{
	if (context.GetConstSharedFragmentPtr<FMassStateTreeSharedFragment>() == nullptr) { return; }

	const TConstArrayView<FMassStateTreeInstanceFragment> stateTreeInstanceList = context.GetFragmentView<FMassStateTreeInstanceFragment>();
	const TConstArrayView<FMassVelocityFragment> velocityFragmentList = context.GetFragmentView<FMassVelocityFragment>();
	const TConstArrayView<FLocomotionFragment> locomotionFragmentList = context.GetFragmentView<FLocomotionFragment>();
	const TArrayView<FMassActorFragment> actorFragmentList = context.GetMutableFragmentView<FMassActorFragment>();

	const int32 numEntities = context.GetNumEntities();

	UMassStateTreeSubsystem& massStateTreeSubsystem = context.GetMutableSubsystemChecked<UMassStateTreeSubsystem>();

	for (int32 i = 0; i < numEntities; ++i)
	{
		AActor* actor = actorFragmentList[i].GetMutable();

		if (!actor) { continue; }

		FStateTreeInstanceData* stateTreeInstanceData = massStateTreeSubsystem.GetInstanceData(stateTreeInstanceList[i].InstanceHandle);
		const FStateTreeExecutionState* executionState = stateTreeInstanceData->GetExecutionState();
		if (!executionState || executionState->ActiveStates.IsEmpty()) { continue; }
		
		const FMassStateTreeSharedFragment& sharedStateTree = context.GetConstSharedFragment<FMassStateTreeSharedFragment>();
		const FStateTreeStateHandle& stateHandle = executionState->ActiveStates.Last();
		const FCompactStateTreeState* stateData = sharedStateTree.StateTree->GetStateFromHandle(stateHandle);
		FMassVelocityFragment velocityFragment = velocityFragmentList[i];


		ACNPCCharacter* character = Cast<ACNPCCharacter>(actor);
		
		UCCharacterMovementComponent* movementComponent = Cast<UCCharacterMovementComponent>(character->GetMovementComponent());
		movementComponent->Velocity = velocityFragment.Value;

		UAnimInstance* skeletalMeshComponent = character->GetSkeletalMeshComponent()->GetAnimInstance();
		UCClusterAnimationInstance* clusterAnimInstance = Cast<UCClusterAnimationInstance>(skeletalMeshComponent);
		
		if (clusterAnimInstance && stateData)
		{
			if (stateData->Name.IsEqual(FName(TEXT("Patrol"))))
			{
				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Jog);
			}
			else
			{
				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Idle);
			}
		}
	}
}

