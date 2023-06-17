// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassCrowdRepresentationActorManagement.h"

#include "CClusterRepresentationActorManagement.generated.h"

/**
*
*/
UCLASS(meta = (DisplayName = "Cluster Visualization"))
class MYPROJECT_API UCClusterRepresentationActorManagement : public UMassCrowdRepresentationActorManagement
{
	GENERATED_BODY()
public:
	virtual EMassActorSpawnRequestAction OnPostActorSpawn(const FMassActorSpawnRequestHandle& SpawnRequestHandle, FConstStructView SpawnRequest, struct FMassEntityManager* EntitySubsystem) const override;

	 virtual void TeleportActor(const FTransform& Transform, AActor& Actor, FMassCommandBuffer& CommandBuffer) const override;
};