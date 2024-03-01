// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"

#include "CWorkerSubsystem.generated.h"

/**
 *
 */
UCLASS()
class MASSMODULE_API UCWorkerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TArray<FMassEntityHandle> ActiveWorkerList;
	TArray<FMassEntityHandle> IdleWorkerList;
	struct FMassEntityManager* EntityManager;

public:
	void OnBeginPlay(UWorld& world);
	void AddWorkerList(TArray<FMassEntityHandle>& newWorkers);
	void AddWorker(const FMassEntityHandle& newWorker);
	bool AssignWorkerToStructure(const FMassEntityHandle& StructureHandle);
	bool RemoveWorkerFromStructure(const FMassEntityHandle& structureHandle, FMassEntityHandle& workerHandle);
	FMassEntityHandle SpawnWorker(const FMassEntityHandle& structureHandle);
};
