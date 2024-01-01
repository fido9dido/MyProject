// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MassCommonTypes.h"
#include "UObject/Interface.h"

#include "ICustomRepresentationActor.generated.h"

UINTERFACE(Blueprintable)
class MASSMODULE_API UCustomRepresentationActor : public UInterface
{
    GENERATED_BODY()
};

class ICustomRepresentationActor
{
	GENERATED_BODY()
	
public:

	virtual void OnGetOrSpawn(struct FMassEntityManager& entityManager, const FMassEntityHandle entityHandle) = 0;
	virtual void OnPostActorSpawn(const FMassEntityHandle structureHandle, const class UCStructureDataAsset* structureData) {};
};