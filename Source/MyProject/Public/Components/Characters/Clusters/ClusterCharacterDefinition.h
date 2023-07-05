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
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCAnimationDataAsset* AnimationData;

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
	int32	CharacterCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleFactor = 1.0f;

	//TODO add a Statetree Here!
};

