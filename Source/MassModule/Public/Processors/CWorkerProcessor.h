// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CWorkerProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCWorkerProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:

	FMassEntityQuery AssignWorkerToStructureQuery;
	FMassEntityQuery FindNearbyResourcesQuery;
	FMassEntityQuery GatherResourcesQuery;
	FMassEntityQuery DepositResourcesQuery;

	class USmartObjectSubsystem* SmartObjectSubsystem;
	class UMassActorSubsystem* MassActorSubsystem;
	class UMassSignalSubsystem* MassSignalSubsystem;
	TArray<FMassEntityHandle> EntitiesToRemove;
public:
	UCWorkerProcessor();

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	void RemoveTagFromEntities(FMassEntityManager& EntityManager, const UScriptStruct* TagType);

	void AssignWorkerToStructure(FMassExecutionContext& Context);
	void RemoveWorkerFromStructure(FMassExecutionContext& Context);

	void FindNearbyResources(struct FMassExecutionContext& Context);
	void GatherResources(struct FMassExecutionContext& Context);
	void ReleaseResource(struct FMassEntityManager& EntityManager, struct FMassEntityHandle& WorkerHandle, struct FSmartObjectClaimHandle& ClaimHandle);
	void DepositResources(struct FMassExecutionContext& Context);
};