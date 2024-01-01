// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassStateTreeTypes.h"
#include "SmartObjectSubsystem.h"
#include "MassSignalSubsystem.h"
#include "MassSmartObjectRequest.h"

#include "FindResourceTask.generated.h"

USTRUCT()
struct MASSMODULE_API FFindResourceTaskInstanceData
{
	GENERATED_BODY()

	/** Result of the candidates search request */
	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectCandidateSlot FoundCandidateSlot;		// @todo: Should turn this in a StateTree result/value.

};


USTRUCT(meta = (DisplayName = "Find Resource"))
struct MASSMODULE_API FFindResourceTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

private:
	using FInstanceDataType = FFindResourceTaskInstanceData;

	TStateTreeExternalDataHandle<struct FWorkerFragment> WorkerFragmentHandle;
	TStateTreeExternalDataHandle<struct FMassSmartObjectUserFragment> SmartObjectUserFragmentHandle;
	TStateTreeExternalDataHandle<class UMassEntitySubsystem> MassEntitySubsystemHandle;
	TStateTreeExternalDataHandle<class USmartObjectSubsystem> SmartObjectSubsystemHandle;
	TStateTreeExternalDataHandle<class UMassActorSubsystem> MassActorSubsystemHandle;
public:
	FFindResourceTask();

protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};









//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "MassStateTreeTypes.h"
//#include "SmartObjectSubsystem.h"
//#include "MassSignalSubsystem.h"
//#include "MassSmartObjectRequest.h"
//
//#include "FindSmartObject.generated.h"
//
//USTRUCT()
//struct MASSMODULE_API FFindSmartObjectInstanceData
//{
//	GENERATED_BODY()
//
//
//	/** Result of the candidates search request */
//	UPROPERTY(VisibleAnywhere, Category = Output)
//	FMassSmartObjectCandidateSlots FoundCandidateSlots;		// @todo: Should turn this in a StateTree result/value.
//
//	UPROPERTY(VisibleAnywhere, Category = Output)
//	bool bHasCandidateSlots = false;
//
//	/** The identifier of the search request send by the task to find candidates */
//	UPROPERTY()
//	FMassSmartObjectRequestID SearchRequestID;
//
//	/** Next update time; task will not do anything when Tick gets called before that time */
//	UPROPERTY()
//	double NextUpdate = 0.;
//
//	///** Last lane where the smart objects were searched. */
//	//UPROPERTY()
//	//FZoneGraphLaneHandle LastLane;
//};
//
//
//USTRUCT(meta = (DisplayName = "Find SmartObject"))
//struct MASSMODULE_API FFindSmartObject : public FMassStateTreeTaskBase
//{
//	GENERATED_BODY()	 
//protected:
//	/** Gameplay tag query for finding matching smart objects. */
//	UPROPERTY(EditAnywhere, Category = Output)
//	FGameplayTagQuery ActivityRequirements;
//
//	/** How frequently to search for new candidates. */
//	UPROPERTY(EditAnywhere, Category = Parameter)
//	float SearchInterval = 10.0f;
//
//private:
//	using FInstanceDataType = FFindSmartObjectInstanceData;
//	//TStateTreeExternalDataHandle<struct FMassZoneGraphLaneLocationFragment, EStateTreeExternalDataRequirement::Optional> LocationHandle;
//
//	TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;
//	TStateTreeExternalDataHandle<UMassSignalSubsystem> MassSignalSubsystemHandle;
//	TStateTreeExternalDataHandle<struct FTransformFragment> EntityTransformHandle;
//	TStateTreeExternalDataHandle<struct FMassSmartObjectUserFragment> SmartObjectUserHandle;
//
//public:
//	FFindSmartObject();
//
//protected:
//	virtual bool Link(FStateTreeLinker& Linker) override;
//	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
//	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
//	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
//	virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override;
//	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
//
//};