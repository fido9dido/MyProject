// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "Engine/SkeletalMesh.h"
#include "Math/Color.h"
#include "UObject/SoftObjectPtr.h"
#include "Components/LODSyncComponent.h"
#include "Characters/CAnimationDataAsset.h"

#include "ClusterCharacterDefinition.generated.h"


USTRUCT(BlueprintType)
struct MYPROJECT_API FClusterMeshDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UCAnimationDataAsset> AnimationData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleWidth;
};

USTRUCT(BlueprintType)
struct MYPROJECT_API FClusterCharacterDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FClusterMeshDefinition MeshDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleFactor = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RayTracingMinLOD = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataAsset> LocomotionAnimSet;

public:

	FClusterCharacterDefinition() = default;

};

USTRUCT(BlueprintType)
struct MYPROJECT_API FSkeletalMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FClusterMeshDefinition MeshDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleFactor = 1.0f;
};