// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CPlacementSubsystem.h"
#include "MassEntityConfigAsset.h"
#include "MassEntityTypes.h"
#include "Utilities/Util.h"
#include "GameSettings/CGameSettings.h"
#include "Actors/CPreviewActor.h"

FMassEntityHandle& UCPlacementSubsystem::GetPreviewHandle()
{
	ensureMsgf(PreviewHandle.IsValid(), TEXT("StructurePreviewHandle is not assigned in game Instance"));
	return PreviewHandle;
}

UMassEntityConfigAsset* UCPlacementSubsystem::GetPreviewConfig()
{
	ensureMsgf(PreviewConfig != nullptr, TEXT("StructurePreviewConfig is not assigned in game Instance"));
	return PreviewConfig;
}

void UCPlacementSubsystem::SetPreviewHandle(const FMassEntityHandle& previewHandle)
{
	PreviewHandle = previewHandle;
}

void UCPlacementSubsystem::AddStructure(const FMassEntityHandle& structureHandle, const UCStructureDataAsset* structureData)
{
	SpawnedStructureDataMap.Add(structureHandle, structureData);
}

void UCPlacementSubsystem::SpawnPreviewEntity(UWorld& world)
{
	TArray<FMassEntityHandle> outEntities = FUtil::SpawnEntity(PreviewConfig, world, 1, TArray<FTransform>());
	PreviewHandle = outEntities[0];
}

void UCPlacementSubsystem::OnBeginPlay(UWorld& world)
{
	SpawnPreviewEntity(world);
}

void UCPlacementSubsystem::EnablePlacement()
{
	OnPlacementEnabled.Broadcast();
	if (ensure(PreviewActor))
	{
		PreviewActor.Get()->OnPreviewEnabled.Broadcast();
	}

	bEnabled = true;
}

void UCPlacementSubsystem::DisablePlacement()
{
	OnPlacementDisabled.Broadcast();

	if (ensure(PreviewActor))
	{
		PreviewActor->OnPreviewDisabled.Broadcast();
	}

	bEnabled = false;
}

bool UCPlacementSubsystem::SpawnedStructureExists(const FMassEntityHandle& structureHandle)
{
	return SpawnedStructureDataMap.Contains(structureHandle);
}

void UCPlacementSubsystem::SetPreviewActor(ACPreviewActor* previewActor)
{
	PreviewActor = previewActor;
}

void UCPlacementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UCGameSettings* gameSettings = GetMutableDefault<UCGameSettings>();
	PreviewConfig = gameSettings->GetPreviewConfig();
}
