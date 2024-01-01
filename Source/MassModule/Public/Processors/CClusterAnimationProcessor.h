// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CClusterAnimationProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCClusterAnimationProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:
	FMassEntityQuery EntityQuery;

public:
	UCClusterAnimationProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	void Tick(FMassExecutionContext& context);

};