// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "ClusterCharacterDefinition.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "CClusterDefinition.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UCClusterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "Client"))
	FSkeletalMeshData MeshData;

	// Optionnally override the min. ray tracing LOD set on the skeleton mesh. Default: -1, use the skeleton mesh value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LOD Sync")
	int32 RayTracingMinLOD = -1;

public:

	/* Returns Character Definition from given indices */
	UFUNCTION(BlueprintCallable)
	FClusterCharacterDefinition GetCharacterDefinition() const;

	/* Finds Unique  DataAsset used in all Definitions. */
	UFUNCTION(BlueprintCallable)
	TArray<UCAnimationDataAsset*> FindAnimationDataAssets() const;

	/* Finds Unique (Outfit/Body/Head) SkeletalMeshes used in all Definitions. */
	UFUNCTION(BlueprintCallable)
	TArray<USkeletalMesh*> FindSkeletalMeshes() const;
};
