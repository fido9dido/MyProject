// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CClusterUpdateProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCClusterUpdateProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:
	FMassEntityQuery EntityQuery;

public:
	UCClusterUpdateProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	void Tick(FMassExecutionContext& Context);
};