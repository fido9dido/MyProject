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
private:
	using FInstanceDataType = FStandTaskInstanceData;

	TStateTreeExternalDataHandle<FTransformFragment> LocationHandle;
	TStateTreeExternalDataHandle<FMassMoveTargetFragment> MoveTargetHandle;
	TStateTreeExternalDataHandle<FMassMovementParameters> MovementParamsHandle;
	TStateTreeExternalDataHandle<UMassSignalSubsystem> MassSignalSubsystemHandle;

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};
