// Fill out your copyright notice in the Description page of Project Settings.

#include "Processors/CClusterUpdateProcessor.h"
#include "Characters/CCharacterMovementComponent.h"
#include "Characters/CAgentCharacter.h"
#include "Characters/Animation/CClusterAnimationInstance.h"
#include <MassStateTreeFragments.h>
#include <MassActorSubsystem.h>
#include <MassCommonTypes.h>
#include <StateTreeTypes.h>
#include <MassMovementFragments.h>
#include <Fragments/LocomotionFragment.h> 
#include <MassExecutionContext.h>
#include <functional>

UCClusterUpdateProcessor::UCClusterUpdateProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Avoidance);
	ExecutionOrder.ExecuteAfter.Add(TEXT("MassStateTreeProcessor"));
	ExecutionOrder.ExecuteAfter.Add(TEXT("CNavMeshPathFollowProcessor"));
	bRequiresGameThreadExecution = true;
}

void UCClusterUpdateProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassStateTreeInstanceFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FLocomotionFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassVelocityFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassForceFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<UMassStateTreeSubsystem>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FMassStateTreeSharedFragment>();
}

void UCClusterUpdateProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCClusterUpdateProcessor::Tick, this, std::placeholders::_1));
}

void UCClusterUpdateProcessor::Tick(FMassExecutionContext& Context)
{
	const TConstArrayView<FMassStateTreeInstanceFragment> stateTreeInstanceList = Context.GetFragmentView<FMassStateTreeInstanceFragment>();
	const TConstArrayView<FLocomotionFragment> locomotionFragmentList = Context.GetFragmentView<FLocomotionFragment>();
	const TArrayView<FMassActorFragment> actorFragmentList = Context.GetMutableFragmentView<FMassActorFragment>();
	TArrayView<FMassVelocityFragment> velocityFragmentList = Context.GetMutableFragmentView<FMassVelocityFragment>();
	TArrayView<FMassForceFragment> forceListFragment = Context.GetMutableFragmentView<FMassForceFragment>();
	UMassStateTreeSubsystem& massStateTreeSubsystem = Context.GetMutableSubsystemChecked<UMassStateTreeSubsystem>();
	const FMassStateTreeSharedFragment& sharedStateTree = Context.GetConstSharedFragment<FMassStateTreeSharedFragment>();

	const int32 numEntities = Context.GetNumEntities();

	for (int32 i = 0; i < numEntities; ++i)
	{
		AActor* actor = actorFragmentList[i].GetMutable();

		if (!actor) { continue; }

		FStateTreeInstanceData* stateTreeInstanceData = massStateTreeSubsystem.GetInstanceData(stateTreeInstanceList[i].InstanceHandle);
		const FStateTreeExecutionState* executionState = stateTreeInstanceData->GetExecutionState();
		
		if (!executionState || executionState->ActiveFrames.IsEmpty()) { continue; }
		
		//No clue about the new changes epic made to S.T. but after debugging this works for now
		const FStateTreeExecutionFrame& lastFrame = executionState->ActiveFrames.Last();
		const FStateTreeStateHandle& stateHandle = lastFrame.ActiveStates.Last();
		const FCompactStateTreeState* stateData = sharedStateTree.StateTree->GetStateFromHandle(stateHandle);
		FMassVelocityFragment& velocityFragment = velocityFragmentList[i];
		FMassForceFragment& forceFragment = forceListFragment[i];
		ACAgentCharacter* character = Cast<ACAgentCharacter>(actor);
		
		UCCharacterMovementComponent* movementComponent = Cast<UCCharacterMovementComponent>(character->GetMovementComponent());
		const float speed = velocityFragment.Value.Length();
		UAnimInstance* skeletalMeshComponent = character->GetSkeletalMeshComponent()->GetAnimInstance();
		UCClusterAnimationInstance* clusterAnimInstance = Cast<UCClusterAnimationInstance>(skeletalMeshComponent);
		
		if (clusterAnimInstance && stateData)
		{
			if (stateData->Name.IsEqual(FName(TEXT("Move"))))
			{
				movementComponent->Velocity = velocityFragment.Value;
				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Jog);
			}
			else 	  
			{	 
				velocityFragment.Value = FVector::ZeroVector;
				forceFragment.Value = FVector::ZeroVector;
				movementComponent->Velocity = FVector::ZeroVector;

				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Idle);

			}
		}
	}
}

