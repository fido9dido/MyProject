// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct MASSMODULE_API FMassUtil
{
	static void SendStructureDataToPreviewEntity(class UCStructureDataAsset* structureData, class UWorld* world);

	static TArray<struct FMassEntityHandle> SpawnEntity(class UMassEntityConfigAsset& entityConfig, class UWorld* world, int32 numToSpawn, const TArray<FTransform>& transformList);
};
