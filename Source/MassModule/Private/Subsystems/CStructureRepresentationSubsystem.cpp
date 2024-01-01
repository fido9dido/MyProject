// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CStructureRepresentationSubsystem.h"
#include <MassRepresentationSubsystem.h>
#include <MassCrowdSpawnerSubsystem.h>
#include "Engine/World.h"

void UCStructureRepresentationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UMassCrowdSpawnerSubsystem::StaticClass());

	Super::Initialize(Collection);

	ActorSpawnerSubsystem = UWorld::GetSubsystem<UMassActorSpawnerSubsystem>(GetWorld());
	ActorSpawnerSubsystem->EnableActorPooling();
}
