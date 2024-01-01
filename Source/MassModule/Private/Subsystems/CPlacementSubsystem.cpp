// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CPlacementSubsystem.h"
#include "MassEntityConfigAsset.h"
#include "MassEntityTypes.h"
#include "Utilities/Util.h"
#include "GameSettings/CGameSettings.h"

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

void UCPlacementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UCGameSettings* gameSettings = GetMutableDefault<UCGameSettings>();
	PreviewConfig = gameSettings->GetPreviewConfig();
}

void UCPlacementSubsystem::Deinitialize()
{
	Super::Deinitialize();
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

void UCPlacementSubsystem::SetPreviewActor(ACPreviewActor* previewActor)
{
	PreviewActor = previewActor;
}


