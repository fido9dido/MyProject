// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CPLayerControllerProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCPLayerControllerProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:
	APlayerController* PlayerController;
public:
	UCPLayerControllerProcessor();
	
protected:
	virtual void Initialize(UObject& Owner);
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	
	void Tick(FMassExecutionContext& context);
};