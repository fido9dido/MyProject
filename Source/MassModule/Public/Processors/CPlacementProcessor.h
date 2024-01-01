// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassProcessor.h"
#include "MassObserverProcessor.h"
#include "CPlacementProcessor.generated.h"

UCLASS()
class MASSMODULE_API UCPlacementProcessor : public UMassProcessor
{
	GENERATED_BODY()

protected:
	FMassEntityQuery EntityQuery;
	TWeakObjectPtr<class ACDefaultPlayerController> PlayerController;
	TWeakObjectPtr<class UCPlacementSubsystem> PlacementSubsystem;

public:
	UCPlacementProcessor();
	
protected:
	virtual void Initialize(UObject& Owner);
	virtual void PostInitProperties() override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	void Tick(FMassExecutionContext& context);

	bool TrySpawnStructure(FMassExecutionContext& Context, const class UCStructureDataAsset* structureData, const class ACPreviewActor& previewActor);

};

template<>
struct TMassExternalSubsystemTraits<UCPlacementSubsystem> final
{
	enum
	{
		GameThreadOnly = true
	};
};