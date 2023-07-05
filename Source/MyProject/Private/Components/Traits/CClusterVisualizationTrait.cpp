// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CClusterVisualizationTrait.h"
#include <MassEntityTemplateRegistry.h>
#include <MassNavigationFragments.h>
#include <MassActorSubsystem.h>
#include <MassLODFragments.h>

void UCClusterVisualizationTrait::BuildTemplate(FMassEntityTemplateBuildContext& buildContext, const UWorld& world) const
{			 
	buildContext.AddFragment<FMassActorFragment>();
	buildContext.AddFragment<FMassViewerInfoFragment>();
	buildContext.AddFragment<FClusterVisualizationFragment>();
	
	FMassEntityManager& entityManager = UE::Mass::Utils::GetEntityManagerChecked(world);
	
	//long story short, I don't want this to be a const but it doesnt work otherwise for now atleast
	FConstSharedStruct paramsFragment = entityManager.GetOrCreateConstSharedFragment(ClusterDataAsset);
	buildContext.AddConstSharedFragment(paramsFragment);

	/*uint32 paramsHash = UE::StructUtils::GetStructCrc32(FConstStructView::Make(PointOfInterestsParams));
	FSharedStruct sharedFragment = entityManager.GetOrCreateSharedFragmentByHash<FPointoOfInterestsSharedFragment>(paramsHash, PointOfInterestsParams);
	buildContext.AddSharedFragment(sharedFragment);*/
}
