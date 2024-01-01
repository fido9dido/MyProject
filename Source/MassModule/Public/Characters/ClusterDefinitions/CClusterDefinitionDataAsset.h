// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "ClusterCharacterDefinition.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "CClusterDefinitionDataAsset.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MASSMODULE_API UCClusterDefinitionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FClusterCharacterDefinition>	CharacterDefinitionList;

	// Optionnally override the min. ray tracing LOD set on the skeleton mesh. Default: -1, use the skeleton mesh value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LOD Sync")
	int32 RayTracingMinLOD = -1;

public:

	/* Returns Character Definition from given indices */
	UFUNCTION(BlueprintCallable)
	FClusterCharacterDefinition GetCharacterDefinition(int32 index) const;

	/* Finds Unique  DataAsset used in all Definitions. */
	UFUNCTION(BlueprintCallable)
	TArray<UCAnimationDataAsset*> FindAnimationDataAssets() const;

	/* Finds Unique (Outfit/Body/Head) SkeletalMeshes used in all Definitions. */
	UFUNCTION(BlueprintCallable)
	TArray<USkeletalMesh*> FindSkeletalMeshes() const;
};
