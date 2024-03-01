// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include <MassSmartObjectTypes.h>
#include <MassStateTreeTypes.h>
#include "SmartObjectRuntime.h"
#include "MassSmartObjectRequest.h"
#include "FindSmartObjectTargetTask.generated.h"


/**
 *	GetInstanceDataType makes an instanced version of this struct in the data asset
 */

USTRUCT()
struct MASSMODULE_API FFindSmartObjectTargetInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Input)
	FSmartObjectClaimHandle ClaimedSlot;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FVector TargetLocation = FVector::ZeroVector;

};

USTRUCT(meta = (DisplayName = "Find SmartObject Target"))
struct MASSMODULE_API FFindSmartObjectTargetTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

private:
	using FInstanceDataType = FFindSmartObjectTargetInstanceData;
	TStateTreeExternalDataHandle<class USmartObjectSubsystem> SmartObjectSubsystemHandle;

public:
	FFindSmartObjectTargetTask();

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); };
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};