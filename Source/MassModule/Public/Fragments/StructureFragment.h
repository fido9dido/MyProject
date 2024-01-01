// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include <SmartObjectTypes.h>
#include "GameplayTagContainer.h"
#include "StructureFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FStructureFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer UserTags;
	
	//Query to find the Resources this structure uses
	UPROPERTY(EditAnywhere)
	FGameplayTagQuery ResourceQuery;

	//Query to Find Structure's slots
	UPROPERTY(EditAnywhere)
	FGameplayTagQuery StructureQuery;

	UPROPERTY(EditAnywhere)
	float BuildingRange = 0.f;
	
	UPROPERTY(EditAnywhere)
	int32 MaxWorkers = 0;

	TArray<FMassEntityHandle> WorkersList;

};
