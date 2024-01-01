// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassCrowdRepresentationActorManagement.h"

#include "CClusterRepresentationActorManagement.generated.h"

/**
*
*/
UCLASS(meta = (DisplayName = "Cluster Representation Actor Management"))
class MASSMODULE_API UCClusterRepresentationActorManagement : public UMassCrowdRepresentationActorManagement
{
	GENERATED_BODY()
public:

	virtual AActor* GetOrSpawnActor(UMassRepresentationSubsystem& representationSubsystem, FMassEntityManager& entityManager
		, const FMassEntityHandle massAgent, const FTransform& transform, const int16 templateActorIndex
		, FMassActorSpawnRequestHandle& spawnRequestHandle, const float priority) const override;

	 virtual void TeleportActor(const FTransform& transform, AActor& actor, FMassCommandBuffer& commandBuffer) const override;
};