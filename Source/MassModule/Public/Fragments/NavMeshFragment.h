// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassObserverProcessor.h"

#include "NavMeshFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FNavMeshFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	
	//agent size is added to Tolerance on Initalization
	UPROPERTY(EditAnywhere)
	float Tolerance = 10;

	//maybe add it in a seperate fragment "CPathCache"
	TArray<FNavPathPoint> PathPoints;
	int32 NextPathNodeIndex = -1;

public:
	void ResetPathCache()
	{ 
		PathPoints.Reset();
		NextPathNodeIndex = -1;
	}
	void SetNewPathCache(const TArray<FNavPathPoint>& path, const int32 nextPathNodeIndex)
	{
		PathPoints = path;
		NextPathNodeIndex = nextPathNodeIndex;
	}
};

UCLASS()
class MASSMODULE_API UCNavMeshFragmentInitializer : public UMassObserverProcessor
{
	GENERATED_BODY()

public:
	UCNavMeshFragmentInitializer();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

protected:
	FMassEntityQuery EntityQuery;

};
