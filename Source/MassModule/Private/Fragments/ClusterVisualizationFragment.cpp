// Fill out your copyright notice in the Description page of Project Settings.

#include "Fragments/ClusterVisualizationFragment.h"

#include "Characters/ClusterDefinitions/CClusterDefinitionDataAsset.h"
#include "MassCrowdRepresentationSubsystem.h"
#include "MassRepresentationTypes.h"
#include "MassRepresentationFragments.h"

UCClusterVisualizationFragmentInitializer::UCClusterVisualizationFragmentInitializer()
	: EntityQuery(*this)
{
	ObservedType = FClusterVisualizationFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void UCClusterVisualizationFragmentInitializer::ConfigureQueries() 
{
	EntityQuery.AddRequirement<FClusterVisualizationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FClusterDataAssetSharedFragment>(EMassFragmentPresence::All);
}

void UCClusterVisualizationFragmentInitializer::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	UMassCrowdRepresentationSubsystem* representationSubsystem = UWorld::GetSubsystem<UMassCrowdRepresentationSubsystem>(entityManager.GetWorld());
	EntityQuery.ForEachEntityChunk(entityManager, context, [&, this, representationSubsystem](FMassExecutionContext& context)
	{						 
		const FClusterDataAssetSharedFragment& clusterDataAssetFragment = context.GetConstSharedFragment<FClusterDataAssetSharedFragment>();
		const UCClusterDefinitionDataAsset* characterDataAsset = clusterDataAssetFragment.ClusterDefinitionData.Get();

		const TArrayView<FClusterVisualizationFragment> visualizationList = context.GetMutableFragmentView<FClusterVisualizationFragment>();
		const TArrayView<FMassRepresentationFragment> representationList = context.GetMutableFragmentView<FMassRepresentationFragment>();
		const int32 numEntities = context.GetNumEntities();
		
		int32 characterDefinitionID = -1;
		int32 numToSpawn = 0;
		int32 maxSpawn = 0;
		for (int32 i = 0; i < numEntities; ++i)
		{
			check(characterDataAsset->CharacterDefinitionList.Num() != 0);

			if (characterDataAsset->CharacterDefinitionList.IsEmpty())
			{
				break;
			}

			if (characterDataAsset->CharacterDefinitionList.IsValidIndex(characterDefinitionID + 1) && numToSpawn >= maxSpawn)
			{
				++characterDefinitionID;
				maxSpawn += characterDataAsset->CharacterDefinitionList[characterDefinitionID].CharacterCount;
			}

			FClusterVisualizationFragment& visualization = visualizationList[i];
			visualization.CharacterDefinitionID = characterDefinitionID;
			visualization.CapsuleHeight = characterDataAsset->CharacterDefinitionList[characterDefinitionID].MeshDefinition.CapsuleHeight;
			++numToSpawn;
		}
	});
};
