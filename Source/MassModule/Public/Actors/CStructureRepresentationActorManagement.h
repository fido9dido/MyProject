// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassCrowdRepresentationActorManagement.h"

#include "CStructureRepresentationActorManagement.generated.h"

/**
*
*/
UCLASS(meta = (DisplayName = "Structure Representation Actor Management"))
class MASSMODULE_API UCStructureRepresentationActorManagement : public UMassCrowdRepresentationActorManagement
{
	GENERATED_BODY()
public:

	virtual AActor* GetOrSpawnActor(UMassRepresentationSubsystem& RepresentationSubsystem, FMassEntityManager& EntitySubsystem
		, const FMassEntityHandle MassAgent, const FTransform& Transform, const int16 TemplateActorIndex
		, FMassActorSpawnRequestHandle& SpawnRequestHandle, const float Priority) const override;

	 virtual void TeleportActor(const FTransform& Transform, AActor& Actor, FMassCommandBuffer& CommandBuffer) const override;
};