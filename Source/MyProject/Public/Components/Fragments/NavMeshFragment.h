// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassObserverProcessor.h"

#include "NavMeshFragment.generated.h"

USTRUCT()
struct MYPROJECT_API FNavMeshFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	
	//Tolerance is initalized and added to agent size
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
class MYPROJECT_API UCNavMeshFragmentInitializer : public UMassObserverProcessor
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

USTRUCT()
struct MYPROJECT_API FPointoOfInterestsSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

// Temporarily public
public:
	// this is the MassSpawner Location, visible for debugging purposes
	UPROPERTY(VisibleAnywhere)
	FVector Location;
	
	UPROPERTY(EditAnywhere)
	float Radius = 1000.f;
public:
	FPointoOfInterestsSharedFragment() = default;

	const FVector& GetLocation() const { return Location; }
	const float& GetRadius() const { return Radius; }
};