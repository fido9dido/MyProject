// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawners/CMassSpawner.h"
#include <MassEntityConfigAsset.h>
#include "Traits/CNavMeshTrait.h"


void ACMassSpawner::BeginPlay()
{	
	//Todo: Implement another way to initalize that shared fragment with the spawner location
	if (EntityTypes.Num() == 0)
	{
		return;
	}
	for (FMassSpawnedEntityType& EntityType : EntityTypes)
	{
		UMassEntityConfigAsset* entityConfig = EntityType.EntityConfig.LoadSynchronous();
		if (entityConfig)
		{
			const UMassEntityTraitBase* trait = entityConfig->FindTrait(UCNavMeshTrait::StaticClass());
			if (!trait) { continue; }
			const UCNavMeshTrait* navMesh = Cast<UCNavMeshTrait>(trait);
			UCNavMeshTrait* navMeshMustable = const_cast<UCNavMeshTrait*>(navMesh);

			navMeshMustable->PointOfInterests.Location = GetActorLocation();
		}
	}
	Super::BeginPlay();
}
