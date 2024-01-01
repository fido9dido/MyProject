// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CNavMeshPathFollowProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCNavMeshPathFollowProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:
	FMassEntityQuery EntityQuery;

public:
	UCNavMeshPathFollowProcessor();

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	void Tick(FMassExecutionContext& Context);
	
	class UMassSignalSubsystem* SignalSubsystem;

	TArray<FMassEntityHandle> EntitiesToSignalPathDone;
};