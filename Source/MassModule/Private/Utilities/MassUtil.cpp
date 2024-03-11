#include "Utilities/MassUtil.h"
#include "MassSpawner.h"
#include "MassSpawnLocationProcessor.h"
#include <MassSpawnerSubsystem.h>
#include <MassEntityConfigAsset.h>
#include <MassEntitySubsystem.h>	
#include "MassEntityManager.h"
#include <Fragments/PreviewSharedFragment.h>
#include <Subsystems/CPlacementSubsystem.h>

void FMassUtil::SendStructureDataToPreviewEntity(UCStructureDataAsset* structureData, UWorld* world)
{
	UCPlacementSubsystem* placementSubsystem = UGameInstance::GetSubsystem<UCPlacementSubsystem>(world->GetGameInstance());
	if (!placementSubsystem) { return; }

	placementSubsystem->GetPreviewActor()->SetStructureData(structureData);

	UMassEntitySubsystem* entitySubsystem = UWorld::GetSubsystem<UMassEntitySubsystem>(world);
	FMassEntityManager& entityManager = entitySubsystem->GetMutableEntityManager();
	entityManager.ForEachSharedFragment<FPreviewSharedFragment>(
		[&](FPreviewSharedFragment& previewSharedFragment)
		{
			previewSharedFragment.StructureDataWeakPtr = structureData;
		});
}

TArray<FMassEntityHandle> FMassUtil::SpawnEntity(UMassEntityConfigAsset& entityConfig, UWorld* world, int32 numToSpawn, const TArray<FTransform>& transformList)
{
	UMassSpawnerSubsystem* spawnerSubsystem = UWorld::GetSubsystem<UMassSpawnerSubsystem>(world);
	const FMassEntityTemplate& entityTemplate = entityConfig.GetOrCreateEntityTemplate(*world);
	
	FMassTransformsSpawnData SpawnLocationData;
	SpawnLocationData.Transforms = transformList; 
	
	TArray<FMassEntityHandle> outEntities;
	spawnerSubsystem->SpawnEntities(entityTemplate.GetTemplateID(), numToSpawn, FConstStructView::Make(SpawnLocationData), UMassSpawnLocationProcessor::StaticClass(), outEntities);

	return outEntities;
}