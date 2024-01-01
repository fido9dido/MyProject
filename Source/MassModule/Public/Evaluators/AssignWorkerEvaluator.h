// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassSmartObjectRequest.h"
#include "MassStateTreeTypes.h"
#include "AssignWorkerEvaluator.generated.h"

/**
 *	GetInstanceDataType makes an instanced version of this struct in the data asset
 */

USTRUCT()
struct MASSMODULE_API FAssignWorkerInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	float Range = 5000.f;

	UPROPERTY()
	TArray<struct FSmartObjectRequestResult> SmartObjectResults;
};

// We are not using this anymore!
USTRUCT(meta = (DisplayName = "Assign Worker to Building"))
struct MASSMODULE_API FAssignWorkerEvaluator : public FMassStateTreeEvaluatorBase
{
	GENERATED_BODY()
private:
	
	using FInstanceDataType = FAssignWorkerInstanceData;

	TStateTreeExternalDataHandle<struct FWorkerFragment> WorkerFragmentHandle;
	TStateTreeExternalDataHandle<struct FMassActorFragment> ActorFragment;
	TStateTreeExternalDataHandle<class UMassActorSubsystem> MassActorSubsystemHandle;
	TStateTreeExternalDataHandle<class USmartObjectSubsystem> SmartObjectSubsystemHandle;
	TStateTreeExternalDataHandle<class UMassEntitySubsystem> MassEntitySubsystemHandle;
	TStateTreeExternalDataHandle<struct FTransformFragment> EntityTransformHandle;
	UPROPERTY(EditAnywhere)
	FGameplayTagQuery GameplayTagQuery;
	
protected:
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};