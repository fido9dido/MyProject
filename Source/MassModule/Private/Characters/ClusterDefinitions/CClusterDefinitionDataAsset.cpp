// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ClusterDefinitions/CClusterDefinitionDataAsset.h"
#include "Engine/SkeletalMesh.h"

FClusterCharacterDefinition UCClusterDefinitionDataAsset::GetCharacterDefinition(int32 index) const
{
	FClusterCharacterDefinition characterDefinition;

	characterDefinition.MeshDefinition = CharacterDefinitionList[index].MeshDefinition;
	characterDefinition.CharacterCount = CharacterDefinitionList[index].CharacterCount;
	characterDefinition.ScaleFactor = CharacterDefinitionList[index].ScaleFactor;

	return characterDefinition;
}

TArray<UCAnimationDataAsset*> UCClusterDefinitionDataAsset::FindAnimationDataAssets() const
{
	TArray<UCAnimationDataAsset*> dataAssets;

	for (const FClusterCharacterDefinition& characterDefinition : CharacterDefinitionList)
	{
		UCAnimationDataAsset* data = characterDefinition.MeshDefinition.AnimationData;
		if (data)
		{
			dataAssets.AddUnique(data);
		}
	}
	
	return dataAssets;

}

TArray<USkeletalMesh*> UCClusterDefinitionDataAsset::FindSkeletalMeshes() const
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
