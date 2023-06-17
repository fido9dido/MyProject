// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Clusters/CClusterDefinition.h"
#include "Engine/SkeletalMesh.h"

FClusterCharacterDefinition UCClusterDefinition::GetCharacterDefinition() const
{
	FClusterCharacterDefinition CharacterDefinition;
	if (MeshData.MeshDefinition.Mesh.LoadSynchronous())
	{
		// Get Body Definition
		CharacterDefinition.MeshDefinition = MeshData.MeshDefinition;

		CharacterDefinition.ScaleFactor = MeshData.ScaleFactor;
	}
	
	return CharacterDefinition;
}

TArray<UCAnimationDataAsset*> UCClusterDefinition::FindAnimationDataAssets() const
{
	TArray<UCAnimationDataAsset*> dataAssets;
	UCAnimationDataAsset* data = MeshData.MeshDefinition.AnimationData.LoadSynchronous();
	if (data)
	{
		dataAssets.AddUnique(data);
	}
	return dataAssets;

}

TArray<USkeletalMesh*> UCClusterDefinition::FindSkeletalMeshes() const
{
	TArray<UCAnimationDataAsset*> dataAssets = FindAnimationDataAssets();

	TArray<USkeletalMesh*> SkeletalMeshes;
	for (UCAnimationDataAsset* dataAsset : dataAssets)
	{
		USkeletalMesh* SkeletalMesh = dataAsset->GetSkeletalMesh();
		if (SkeletalMesh)
		{
			SkeletalMeshes.AddUnique(SkeletalMesh);
		}
	}

	return SkeletalMeshes;
}
