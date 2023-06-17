// Fill out your copyright notice in the Description page of Project Settings.

#include "Fragments/ClusterVisualizationFragment.h"

#include "Characters/Clusters/CClusterDefinition.h"
#include "MassCrowdRepresentationSubsystem.h"
#include "MassRepresentationTypes.h"
#include "MassRepresentationFragments.h"
#include "Systems/CClusterSettings.h"
#include "Characters/Clusters/CNPCCharacter.h"

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
}

void UCClusterVisualizationFragmentInitializer::Execute(FMassEntityManager& entityManager, FMassExecutionContext& context)
{
	UMassCrowdRepresentationSubsystem* representationSubsystem = UWorld::GetSubsystem<UMassCrowdRepresentationSubsystem>(entityManager.GetWorld());

	EntityQuery.ForEachEntityChunk(entityManager, context, [&, this, representationSubsystem](FMassExecutionContext& context)
	{
		const TArrayView<FClusterVisualizationFragment> visualizationList = context.GetMutableFragmentView<FClusterVisualizationFragment>();
		const TArrayView<FMassRepresentationFragment> representationList = context.GetMutableFragmentView<FMassRepresentationFragment>();
		const int32 numEntities = context.GetNumEntities();

		for (int32 i = 0; i < numEntities; ++i)
		{
			// Try to get the data asset from the crowd character CDO
			ACNPCCharacter* clusterCharacterCDO = nullptr;
			UCClusterDefinition* characterDataAsset = nullptr;
			TSubclassOf<AActor> templateActorClass = representationSubsystem->GetTemplateActorClass(representationList[i].HighResTemplateActorIndex);
			if (templateActorClass)
			{
				clusterCharacterCDO = Cast<ACNPCCharacter>(templateActorClass->GetDefaultObject());
				if (clusterCharacterCDO)
				{
					characterDataAsset = clusterCharacterCDO->ClusterCharacterData.Get();

					FClusterVisualizationFragment& visualization = visualizationList[i];

					visualization.CapsuleHeight = characterDataAsset->MeshData.MeshDefinition.CapsuleHeight;
				}
			}
		}
	});
};
