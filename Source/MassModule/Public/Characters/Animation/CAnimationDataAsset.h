// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "Engine/SkeletalMesh.h"

#include "CAnimationDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FAnimationSequenceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> AnimationSequence = nullptr;

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite)
	bool bLooping = true;

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite)
	bool bUseCustomRange = false;

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite, meta = (EditCondition = "bUseCustomRange"))
	int32 StartFrame = 0;

	UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite, meta = (EditCondition = "bUseCustomRange"))
	int32 EndFrame = 1;

};

UCLASS(Blueprintable, BlueprintType)
class MASSMODULE_API UCAnimationDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	* SkeletalMesh to bake animations from.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh", meta = (AssetBundles = "Client"))
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<FAnimationSequenceInfo> AnimationSequences;

public:

	int32 GetIndexFromAnimSequence(const UAnimSequence* sequence) 
	{
		int32 outIndex = 0;

		int32 numSequences = AnimationSequences.Num();

		// We can store a sequence to index map for a faster search
		for (int32 currentIndex = 0; currentIndex < numSequences; ++currentIndex)
		{
			const FAnimationSequenceInfo& sequenceInfo = AnimationSequences[currentIndex];
			if (sequenceInfo.AnimationSequence == sequence)
			{
				outIndex = currentIndex;
				break;
			}
		}
		return outIndex;
	}

	USkeletalMesh* GetSkeletalMesh();
};
