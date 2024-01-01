// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Animation/CAnimationDataAsset.h"

USkeletalMesh* UCAnimationDataAsset::GetSkeletalMesh()
{
	USkeletalMesh* result = nullptr;
	if (SkeletalMesh.ToSoftObjectPath().IsValid())
	{
		result = SkeletalMesh.Get();
		if (!result)
		{
			result = Cast<USkeletalMesh>(SkeletalMesh.ToSoftObjectPath().TryLoad());
			if (!ensureMsgf(result, TEXT("Failed to load SkeletalMesh soft pointer %s in UCAnimationDataAsset"), *SkeletalMesh.ToString()))
			{
				result = nullptr;
			}
		}
	}
	return result;
}
