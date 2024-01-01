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

	TArray<FMassEntityHandle> ActiveWorkers;
	TArray<FMassEntityHandle> IdleWorkers;
	struct FMassEntityManager* EntityManager;
public:
	void OnBeginPlay(UWorld& world);
	FMassEntityHandle SpawnWorker(const FMassEntityHandle& structureHandle);
	void AddWorkers(TArray<FMassEntityHandle>& newWorkers);
	void AddWorker(const FMassEntityHandle& newWorker);
	bool AssignWorkerToStructure(FMassEntityHandle& StructureHandle);
	bool RemoveWorkerFromStructure(FMassEntityHandle& structureHandle, FMassEntityHandle& workerHandle);
	void RemoveWorkerFromStructure(FMassEntityHandle& structureHandle);
};
