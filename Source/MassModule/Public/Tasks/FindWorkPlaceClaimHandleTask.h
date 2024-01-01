// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassStateTreeTypes.h"
#include "SmartObjectSubsystem.h"
#include "MassSignalSubsystem.h"
#include "MassSmartObjectRequest.h"

#include "FindWorkPlaceClaimHandleTask.generated.h"

USTRUCT()
struct MASSMODULE_API FFindWorkPlaceClaimHandleInstanceData
{
	GENERATED_BODY()

	/** Result of the candidates search request */
	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectClaimHandle ClaimedSlot;

};


USTRUCT(meta = (DisplayName = "Find Workplace Claim Handle"))
struct MASSMODULE_API FFindWorkPlaceClaimHandleTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

private:
	using FInstanceDataType = FFindWorkPlaceClaimHandleInstanceData;

	TStateTreeExternalDataHandle<struct FWorkerFragment> WorkerFragmentHandle;
	TStateTreeExternalDataHandle<class UMassEntitySubsystem> MassEntitySubsystemHandle;

public:
	FFindWorkPlaceClaimHandleTask();

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};
