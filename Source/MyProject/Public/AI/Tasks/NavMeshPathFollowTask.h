// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include "MassStateTreeTypes.h"
#include "StateTreeTypes.h"
#include <MassMovementTypes.h>

#include <MassCommonFragments.h>
#include <MassNavigationFragments.h>
#include <MassMovementFragments.h>
#include <Fragments/NavMeshFragment.h>
#include <MassActorSubsystem.h>

#include "NavMeshPathFollowTask.generated.h"

USTRUCT()
struct MYPROJECT_API FMoveTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = Parameter)
	FMassMovementStyleRef MovementStyle;

	UPROPERTY(EditAnywhere, Category = Parameter)
	float SpeedScale = 1.0f;
};

/**
 * 
 */

USTRUCT(meta = (DisplayName = "NavMesh Path Follow"))
struct MYPROJECT_API FNavMeshPathFollowTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

protected:
	//LocationHandle
	TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
	TStateTreeExternalDataHandle<FMassMoveTargetFragment> MoveTargetHandle;
	TStateTreeExternalDataHandle<FAgentRadiusFragment> AgentRadiusHandle;
	TStateTreeExternalDataHandle<FMassMovementParameters> MovementParamsHandle;
	TStateTreeExternalDataHandle<FNavMeshFragment> NavMeshFragmentHandle;
	TStateTreeExternalDataHandle<FMassActorFragment> MassActorFragmentHandle;

	//agent radius is added to tolerance and we use it to complete task
	UPROPERTY(EditAnywhere)
	float Tolerance = 10.f;
	
public:	 
	FNavMeshPathFollowTask();

protected:
	
	virtual const UStruct* GetInstanceDataType() const override { return FMoveTaskInstanceData::StaticStruct(); };

	[[nodiscard]] virtual bool Link(FStateTreeLinker& linker) override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& context, const float deltaTime) const;

	[[nodiscard]] EStateTreeRunStatus RequetNewPath(FStateTreeExecutionContext& context) const;

	[[nodiscard]] EStateTreeRunStatus UpdateMoveTarget(FStateTreeExecutionContext& context) const;

};
