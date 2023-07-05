// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Clusters/ClusterCharacterDefinition.h"
#include "Characters/Clusters/CClusterDefinitionDataAsset.h"
#include "MassEntityTypes.h"
#include "MassObserverProcessor.h"

#include "ClusterVisualizationFragment.generated.h"

struct FMassEntityQuery;

USTRUCT()
struct MYPROJECT_API FClusterVisualizationFragment : public FMassFragment
{
	GENERATED_BODY()

public:

	int32 CharacterDefinitionID = -1;

	// Visible for debugging purposes
	UPROPERTY(VisibleAnywhere)
	float CapsuleHeight;

};

USTRUCT()
struct MYPROJECT_API FClusterDataAssetSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<UCClusterDefinitionDataAsset> ClusterDefinitionData;

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