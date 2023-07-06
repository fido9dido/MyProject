// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include "MassStateTreeTypes.h"

#include <MassCommonFragments.h>
#include <MassMovementFragments.h>
#include <MassSignalSubsystem.h>
#include <MassNavigationFragments.h>
#include "NavMeshStandTask.generated.h"

USTRUCT()
struct MASSMODULE_API FStandTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	float Duration = 0.0f;

	UPROPERTY()
	float Time = 0.0f;
};

// Similar behaviour to ZG Stand
USTRUCT(meta = (DisplayName = "NavMesh Stand"))
struct MASSMODULE_API FNavMeshStandTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

protected:
	virtual bool Link(FStateTreeLinker& linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FStandTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& context, const float deltaTime) const override;

	TStateTreeExternalDataHandle<FTransformFragment> LocationHandle;
	TStateTreeExternalDataHandle<FMassMoveTargetFragment> MoveTargetHandle;
	TStateTreeExternalDataHandle<FMassMovementParameters> MovementParamsHandle;
	TStateTreeExternalDataHandle<UMassSignalSubsystem> MassSignalSubsystemHandle;
};
