// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MassCommonTypes.h"
#include "UObject/Interface.h"

#include "IClusterActor.generated.h"

UINTERFACE(Blueprintable)
class MYPROJECT_API UClusterActor : public UInterface
{
    GENERATED_BODY()
};

class IClusterActor
{
	GENERATED_BODY()
	
public:

	virtual void OnGetOrSpawn(struct FMassEntityManager* EntitySubsystem, const FMassEntityHandle MassAgent) = 0;
};