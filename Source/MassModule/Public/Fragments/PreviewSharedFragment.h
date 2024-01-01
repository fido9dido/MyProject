// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include <DataAssets/CStructureDataAsset.h> 
#include "Actors/CPreviewActor.h"
#include "PreviewSharedFragment.generated.h"


USTRUCT()
struct MASSMODULE_API FPreviewSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UCStructureDataAsset> StructureDataWeakPtr;
};
