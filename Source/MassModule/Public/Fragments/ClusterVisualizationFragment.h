// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/ClusterDefinitions/ClusterCharacterDefinition.h"
#include "Characters/ClusterDefinitions/CClusterDefinitionDataAsset.h"
#include "MassEntityTypes.h"
#include "MassObserverProcessor.h"

#include "ClusterVisualizationFragment.generated.h"

struct FMassEntityQuery;

USTRUCT()
struct MASSMODULE_API FClusterTag : public FMassTag
{
	GENERATED_BODY()

	FClusterTag() {}

};

USTRUCT()
struct MASSMODULE_API FClusterVisualizationFragment : public FMassFragment
{
	GENERATED_BODY()

public:

	int32 CharacterDefinitionID = -1;

	// Visible for debugging purposes
	UPROPERTY(VisibleAnywhere)
	float CapsuleHeight = 0.f;

};

USTRUCT()
struct MASSMODULE_API FClusterDataAssetSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<UCClusterDefinitionDataAsset> ClusterDefinitionData;

};

UCLASS()
class MASSMODULE_API UCClusterVisualizationFragmentInitializer : public UMassObserverProcessor
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