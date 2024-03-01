// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CWorkerSubsystem.h"
#include "Engine/GameInstance.h"
#include <Subsystems/CPlacementSubsystem.h>
#include <GameSettings/CGameSettings.h>
#include <MassCommonFragments.h>
#include <Utilities/Util.h>
#include <MassEntitySubsystem.h>
#include <MassEntityConfigAsset.h>
#include <Fragments/StructureFragment.h>
#include <Fragments/WorkerFragment.h>
#include <Tags/StructureTags.h>


void UCWorkerSubsystem::OnBeginPlay(UWorld& world)
{		   
	UMassEntitySubsystem* entitySubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	EntityManager = &entitySubsystem->GetMutableEntityManager();
}


void UCWorkerSubsystem::AddWorkerList(TArray<FMassEntityHandle>& newWorkers)
{
	while (!newWorkers.IsEmpty())
	{
		IdleWorkerList.Emplace(newWorkers.Pop());
	}
}

void UCWorkerSubsystem::AddWorker(const FMassEntityHandle& newWorker)
{
	IdleWorkerList.Emplace(newWorker);
}

bool UCWorkerSubsystem::AssignWorkerToStructure(const FMassEntityHandle& structureHandle)
{
	if (IdleWorkerList.IsEmpty())
	{
		IdleWorkerList.Emplace(SpawnWorker(structureHandle));
	}

	FStructureFragment* structureFragment = EntityManager->GetFragmentDataPtr<FStructureFragment>(structureHandle);
	if (!ensureMsgf(structureFragment, TEXT("UCWorkerSubsystem::AssignWorkerToStructure structure doesnt have structureFragment, ensure the entity has FStructureFragment")))
	{ 
		return false; 
	}

	FMassEntityHandle workerHandle = IdleWorkerList.Pop();
	ActiveWorkerList.Emplace(workerHandle);

	structureFragment->WorkersList.Emplace(workerHandle);

	FWorkerFragment* workerFragment = EntityManager->GetFragmentDataPtr<FWorkerFragment>(workerHandle);
	if (!ensureMsgf(workerFragment, TEXT("UCWorkerSubsystem::AssignWorkerToStructure worker entity doesnt have workerFragment, ensure the entity has FWorkerFragment")))
	{
		return false;
	}

	workerFragment->WorkplaceData.StructureHandle = structureHandle;
	EntityManager->AddTagToEntity(workerHandle, FAddWorkerToStructureTag::StaticStruct());
	return true;
}

bool UCWorkerSubsystem::RemoveWorkerFromStructure(const FMassEntityHandle& structureHandle, FMassEntityHandle& workerHandle)
{
//	bool bStructureExists = StructureWorkerMap.Contains(structureHandle);
	//if (bStructureExists)
	//{
	//	int32 numRemoved = StructureWorkerMap[structureHandle].RemoveSingleSwap(workerHandle);
	//	if (numRemoved == 0) { return false; }
		
	//	if (StructureWorkerMap[structureHandle].IsEmpty())
	//	{
	//		StructureWorkerMap.Remove(structureHandle);
	//	}

	//	FStructureFragment* structureFragment = EntityManager->GetFragmentDataPtr<FStructureFragment>(structureHandle);
	//	if (!ensureMsgf(structureFragment, TEXT("structureFragment is null in UCStructureInfoWidget, ensure the entity has FStructureFragment"))) { return false; }

	//	ActiveWorkers.RemoveSingleSwap(workerHandle);
	//	IdleWorkers.Emplace(workerHandle);
	//	structureFragment->WorkersList.Remove(workerHandle);
	//}
	return false;
}

FMassEntityHandle UCWorkerSubsystem::SpawnWorker(const FMassEntityHandle& structureHandle)
{
	UCGameSettings* gameSettings = GetMutableDefault<UCGameSettings>();
	TArray<FMassEntityHandle> result;
	ensure(GetWorld());

	if (UWorld* world = GetWorld())
	{
		FTransformFragment& transformFragment = EntityManager->GetFragmentDataChecked<FTransformFragment>(structureHandle);
		FTransform spawnLocation = transformFragment.GetTransform();
		spawnLocation.AddToTranslation(FVector(0.f, 50.f, 0.f));
		TArray<FTransform> structureTransform({ spawnLocation });

		result = FUtil::SpawnEntity(gameSettings->WorkerEntityAsset.LoadSynchronous(), *world, 1, structureTransform);
	}

	return result.Pop();
}