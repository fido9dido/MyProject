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
	buildContext.AddTag<FClusterTag>();
	
	FMassEntityManager& entityManager = UE::Mass::Utils::GetEntityManagerChecked(world);

	FConstSharedStruct paramsFragment = entityManager.GetOrCreateConstSharedFragment(ClusterDataAsset);
	buildContext.AddConstSharedFragment(paramsFragment);

}
