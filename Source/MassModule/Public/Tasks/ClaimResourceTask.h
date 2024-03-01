// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include <MassSmartObjectTypes.h>
#include <MassStateTreeTypes.h>
#include "SmartObjectRuntime.h"
#include "MassSmartObjectRequest.h"
#include "ClaimResourceTask.generated.h"


/**
 *	GetInstanceDataType makes an instanced version of this struct in the data asset
 */

USTRUCT()
struct MASSMODULE_API FClaimResourceInstanceData
{
	GENERATED_BODY()
	/** Result of the candidates search request (Input) */
	UPROPERTY(VisibleAnywhere, Category = Input)
	FSmartObjectCandidateSlot FoundCandidateSlot;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectClaimHandle ClaimHandle;

};

USTRUCT(meta = (DisplayName = "Claim Resource"))
struct MASSMODULE_API FClaimResourceTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

private:
	using FInstanceDataType = FClaimResourceInstanceData;

	TStateTreeExternalDataHandle<struct FWorkerFragment> WorkerFragmentHandle;
	TStateTreeExternalDataHandle<class UMassEntitySubsystem> MassEntitySubsystemHandle;
	TStateTreeExternalDataHandle<class USmartObjectSubsystem> SmartObjectSubsystemHandle;

	float InteractionCooldown = 0.f;

public:
	FClaimResourceTask();

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); };
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
};