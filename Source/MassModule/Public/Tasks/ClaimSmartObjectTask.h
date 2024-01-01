// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include <MassSmartObjectTypes.h>
#include <MassStateTreeTypes.h>
#include "SmartObjectRuntime.h"
#include "MassSmartObjectRequest.h"
#include "ClaimSmartObjectTask.generated.h"


/**
 * TODO delete
 */

USTRUCT()
struct MASSMODULE_API FClaimSmartObjectInstanceData
{
	GENERATED_BODY()
	/** Result of the candidates search request (Input) */
	UPROPERTY(VisibleAnywhere, Category = Input)
	FSmartObjectCandidateSlot FoundCandidateSlot;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectClaimHandle ClaimHandle;

};

 // Claim SmartObject Stored at FSmartObjectQueryResultFragment
USTRUCT(meta = (DisplayName = "Claim SmartObject Task"))
struct MASSMODULE_API FClaimSmartObjectTask: public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	FClaimSmartObjectTask();
private:
	using FInstanceDataType = FClaimSmartObjectInstanceData;

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); };
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
private:
	TStateTreeExternalDataHandle<class UMassEntitySubsystem> MassEntitySubsystemHandle;
	TStateTreeExternalDataHandle<class USmartObjectSubsystem> SmartObjectSubsystemHandle;

	float InteractionCooldown = 0.f;
};