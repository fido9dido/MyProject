// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSpawner.h"
#include "CMassSpawner.generated.h"

/**
 * Will Act as a point of interest for NPC
 */
UCLASS()
class MYPROJECT_API ACMassSpawner : public AMassSpawner
{
	GENERATED_BODY()
public:
	ACMassSpawner();
	
	virtual void BeginPlay() override;
};
