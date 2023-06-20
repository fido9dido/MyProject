// Fill out your copyright notice in the Description page of Project Settings.

#include "Fragments/NavMeshFragment.h"


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

void UCNavMeshFragmentInitializer::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	EntityQuery.ForEachEntityChunk(entityManager, context, [&, this](FMassExecutionContext& context)
		{
			const TArrayView<FNavMeshFragment> navMeshFragmentList = context.GetMutableFragmentView<FNavMeshFragment>();
			const TConstArrayView<FAgentRadiusFragment> agentRadiusFragmentList = context.GetFragmentView<FAgentRadiusFragment>();
			const int32 numEntities = context.GetNumEntities();

			for (int32 i = 0; i < numEntities; ++i)
			{
				FNavMeshFragment& navMesh = navMeshFragmentList[i];
				const FAgentRadiusFragment& agentRadiusFragment = agentRadiusFragmentList[i];
				navMesh.Tolerance += agentRadiusFragment.Radius;
			}
		});
}

