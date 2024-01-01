// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
#include "StructureTags.generated.h"

USTRUCT()
struct MASSMODULE_API FAddWorkerToStructureTag : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT()
struct MASSMODULE_API FRemoveWorkerFromStructureTag : public FMassTag
{
	GENERATED_BODY()
};


USTRUCT()
struct MASSMODULE_API FStructurePreviewTag : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT()
struct MASSMODULE_API FConstructionBeginTag : public FMassTag
{
	GENERATED_BODY()
};
