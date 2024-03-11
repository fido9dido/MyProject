// Fill out your copyright notice in the Description page of Project Settings.

#include "Processors/CPlacementProcessor.h"
#include "Characters/CTopDownPlayerController.h"
#include "GameSettings/CGameSettings.h"
#include "Fragments/PreviewSharedFragment.h"
#include "Subsystems/CPlacementSubsystem.h"
#include "Tags/StructureTags.h"
#include "Actors/CPreviewActor.h"
#include <Utilities/MassUtil.h>
#include <MassExecutionContext.h>
#include <Kismet/GameplayStatics.h>
#include <functional>
#include <MassActorSubsystem.h>
#include <Actors/CStructureActor.h>

UCPlacementProcessor::UCPlacementProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteBefore.Add(TEXT("CClusterUpdateProcessor"));
	ExecutionOrder.ExecuteBefore.Add(TEXT("CNavMeshPathFollowProcessor"));

	bRequiresGameThreadExecution = true;
}

void UCPlacementProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);

	UWorld* world = GetWorld();
	PlayerController = Cast<ACDefaultPlayerController>(UGameplayStatics::GetPlayerController(world, 0));
	PlacementSubsystem = world->GetGameInstance()->GetSubsystem<UCPlacementSubsystem>();
	
	UCGameSettings* gameSettings = GetMutableDefault<UCGameSettings>();
	ACPreviewActor* previewActor = world->SpawnActor<ACPreviewActor>(gameSettings->GetPreviewActorClass());

	if (ensureMsgf(previewActor, TEXT("Preview Actor failed to spawn")))
	{
		previewActor->SetActive(false);
		previewActor->OnPreviewEnabled.AddUObject(PlayerController.Get(), &ACDefaultPlayerController::InputModeGameOnly);
		previewActor->OnPreviewDisabled.AddUObject(PlayerController.Get(), &ACDefaultPlayerController::InputModeDefault);
	}	 

	PlacementSubsystem->SetPreviewActor(previewActor);
}

void UCPlacementProcessor::ConfigureQueries()
{
	EntityQuery.AddSharedRequirement<FPreviewSharedFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<UCPlacementSubsystem>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddTagRequirement<FStructurePreviewTag>(EMassFragmentPresence::All);
}

void UCPlacementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, std::bind(&UCPlacementProcessor::Tick, this, std::placeholders::_1));
	
}

void UCPlacementProcessor::Tick(FMassExecutionContext& Context)
{
	ACPreviewActor* previewActor = PlacementSubsystem->GetPreviewActor();
	if (!previewActor || !previewActor->IsActive()) { return; }

	FPreviewSharedFragment& peviewSharedFragment = Context.GetMutableSharedFragment<FPreviewSharedFragment>();
	UCStructureDataAsset* structureData = peviewSharedFragment.StructureDataWeakPtr.Get();
	
	if (structureData && structureData->IsValid())
	{		  	
		FHitResult hitResult;
		
		if (!GetValidLocation(hitResult)) { return; };

		previewActor->SetActorLocation(hitResult.Location);

		if (!previewActor->IsSpawnable()) { return; }
		
		TrySpawnStructure(Context, structureData, *previewActor);
	}

	if (previewActor->IsActive()) 
	{
		previewActor->OnPreviewDisabled.Broadcast();
	}
}

bool UCPlacementProcessor::GetValidLocation(FHitResult& outHitResult)
{
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, outHitResult);
	
	if (!outHitResult.IsValidBlockingHit())
	{
		return false;
	}

	return true;
}

bool UCPlacementProcessor::TrySpawnStructure(FMassExecutionContext& Context, const UCStructureDataAsset* structureData, const ACPreviewActor& previewActor)
{
	UMassEntityConfigAsset* structureConfig = structureData->EntityConfig;
	TArray<FTransform> transformList({ previewActor.GetTransform() });
	TArray<FMassEntityHandle> structureHandle = FMassUtil::SpawnEntity(*structureConfig, GetWorld(), 1, transformList);
	
	if (structureHandle.IsEmpty()) { return false; }
	PlacementSubsystem->AddStructure(structureHandle[0], structureData);

	return true;
}

