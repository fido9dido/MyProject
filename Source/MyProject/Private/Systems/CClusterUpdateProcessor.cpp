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
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Avoidance);
	ExecutionOrder.ExecuteAfter.Add(TEXT("MassStateTreeProcessor"));
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

void UCClusterUpdateProcessor::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	EntityQuery.ForEachEntityChunk(entityManager, context, std::bind(&UCClusterUpdateProcessor::Tick, this, std::placeholders::_1));
}

void UCClusterUpdateProcessor::Tick(FMassExecutionContext& context)
{
	if (context.GetConstSharedFragmentPtr<FMassStateTreeSharedFragment>() == nullptr) { return; }

	const TConstArrayView<FMassStateTreeInstanceFragment> stateTreeInstanceList = context.GetFragmentView<FMassStateTreeInstanceFragment>();
	const TConstArrayView<FLocomotionFragment> locomotionFragmentList = context.GetFragmentView<FLocomotionFragment>();
	const TArrayView<FMassActorFragment> actorFragmentList = context.GetMutableFragmentView<FMassActorFragment>();
	TArrayView<FMassVelocityFragment> velocityFragmentList = context.GetMutableFragmentView<FMassVelocityFragment>();
	TArrayView<FMassForceFragment> forceListFragment = context.GetMutableFragmentView<FMassForceFragment>();

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
		FMassVelocityFragment& velocityFragment = velocityFragmentList[i];
		FMassForceFragment& forceFragment = forceListFragment[i];
		ACNPCCharacter* character = Cast<ACNPCCharacter>(actor);
		
		UCCharacterMovementComponent* movementComponent = Cast<UCCharacterMovementComponent>(character->GetMovementComponent());
		const float speed = velocityFragment.Value.Length();
		UAnimInstance* skeletalMeshComponent = character->GetSkeletalMeshComponent()->GetAnimInstance();
		UCClusterAnimationInstance* clusterAnimInstance = Cast<UCClusterAnimationInstance>(skeletalMeshComponent);
		
		if (clusterAnimInstance && stateData)
		{
			if (stateData->Name.IsEqual(FName(TEXT("Patrol"))))
			{
				movementComponent->Velocity = velocityFragment.Value;
				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Jog);
			}
			else if (stateData->Name.IsEqual(FName(TEXT("Idle"))))
			{	 
				// temporary fix until I fix avoidance, currently avoidance mutate these values making character moves when they are standing
				velocityFragment.Value = FVector::ZeroVector;
				forceFragment.Value = FVector::ZeroVector;

				movementComponent->Velocity = FVector::ZeroVector;
				clusterAnimInstance->SetLocomotionState(ECLocomotionState::Idle);

			}
		}
	}
}

