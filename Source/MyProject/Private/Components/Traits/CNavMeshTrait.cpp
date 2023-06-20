// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CNavMeshTrait.h"
#include "Fragments/NavMeshFragment.h"
#include <MassEntityTemplateRegistry.h>
#include <MassNavigationFragments.h>

void UCNavMeshTrait::BuildTemplate(FMassEntityTemplateBuildContext& buildContext, const UWorld& world) const
{			 
	buildContext.AddFragment<FNavMeshFragment>();
	buildContext.AddFragment<FMassMoveTargetFragment>();
	
	FMassEntityManager& entityManager = UE::Mass::Utils::GetEntityManagerChecked(world);
	
	//long story short, I don't want this to be a const but it doesnt work otherwise for now atleast
	FConstSharedStruct paramsFragment = entityManager.GetOrCreateConstSharedFragment(PointOfInterests);
	buildContext.AddConstSharedFragment(paramsFragment);

	/*uint32 paramsHash = UE::StructUtils::GetStructCrc32(FConstStructView::Make(PointOfInterestsParams));
	FSharedStruct sharedFragment = entityManager.GetOrCreateSharedFragmentByHash<FPointoOfInterestsSharedFragment>(paramsHash, PointOfInterestsParams);
	buildContext.AddSharedFragment(sharedFragment);*/
}
