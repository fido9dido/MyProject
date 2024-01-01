// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "Engine/SkeletalMesh.h"
#include "Math/Color.h"
#include "UObject/SoftObjectPtr.h"
#include "Components/LODSyncComponent.h"
#include "Characters/Animation/CAnimationDataAsset.h"

#include "ClusterCharacterDefinition.generated.h"

USTRUCT(BlueprintType)
struct MASSMODULE_API FClusterMeshDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCAnimationDataAsset* AnimationData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleWidth = 0.f;
};

USTRUCT(BlueprintType)
struct MASSMODULE_API FClusterCharacterDefinition
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

