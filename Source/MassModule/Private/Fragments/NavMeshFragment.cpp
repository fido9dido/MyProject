// Fill out your copyright notice in the Description page of Project Settings.

#include "Fragments/NavMeshFragment.h"
#include <MassExecutionContext.h>

UCNavMeshFragmentInitializer::UCNavMeshFragmentInitializer()
	: EntityQuery(*this)
{
	ObservedType = FNavMeshFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void UCNavMeshFragmentInitializer::ConfigureQueries()
{
	EntityQuery.AddRequirement<FNavMeshFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FAgentRadiusFragment>(EMassFragmentAccess::ReadOnly);

}

void UCNavMeshFragmentInitializer::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&, this](FMassExecutionContext& Context)
		{
			const TArrayView<FNavMeshFragment> navMeshFragmentList = Context.GetMutableFragmentView<FNavMeshFragment>();
			const TConstArrayView<FAgentRadiusFragment> agentRadiusFragmentList = Context.GetFragmentView<FAgentRadiusFragment>();
			const int32 numEntities = Context.GetNumEntities();

			for (int32 i = 0; i < numEntities; ++i)
			{
				FNavMeshFragment& navMesh = navMeshFragmentList[i];
				const FAgentRadiusFragment& agentRadiusFragment = agentRadiusFragmentList[i];
				navMesh.Tolerance += agentRadiusFragment.Radius;
			}
		});
}

