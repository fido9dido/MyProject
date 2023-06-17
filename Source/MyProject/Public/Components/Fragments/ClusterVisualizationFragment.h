// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Clusters/ClusterCharacterDefinition.h"
#include "MassEntityTypes.h"
#include "MassObserverProcessor.h"

#include "ClusterVisualizationFragment.generated.h"

struct FMassEntityQuery;

USTRUCT()
struct MYPROJECT_API FClusterVisualizationFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	
	// Visible for debugging purposes
	UPROPERTY(VisibleAnywhere)
	float CapsuleHeight;

};

UCLASS()
class MYPROJECT_API UCClusterVisualizationFragmentInitializer : public UMassObserverProcessor
{
	GENERATED_BODY()
	
public:
	UCClusterVisualizationFragmentInitializer();	

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

protected:
	FMassEntityQuery EntityQuery;

};