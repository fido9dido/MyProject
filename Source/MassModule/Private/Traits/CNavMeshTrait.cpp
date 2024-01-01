// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CNavMeshTrait.h"
#include "Fragments/NavMeshFragment.h"
#include <MassEntityTemplateRegistry.h>
#include <MassNavigationFragments.h>

void UCNavMeshTrait::BuildTemplate(FMassEntityTemplateBuildContext& buildContext, const UWorld& world) const
{			 
	buildContext.AddFragment(FConstStructView::Make(NavMeshFragment));
	buildContext.AddFragment<FMassMoveTargetFragment>();
	
	FMassEntityManager& entityManager = UE::Mass::Utils::GetEntityManagerChecked(world);
	uint32 paramsHash = UE::StructUtils::GetStructCrc32(FConstStructView::Make(PointOfInterests));
	//long story short, I don't want this to be a const but it doesnt work because of a weird bug, even when i asked i was told it should work!!!
	FConstSharedStruct paramsFragment = entityManager.GetOrCreateConstSharedFragment(PointOfInterests);
	buildContext.AddConstSharedFragment(paramsFragment);

	/*uint32 paramsHash = UE::StructUtils::GetStructCrc32(FConstStructView::Make(PointOfInterestsParams));
	FSharedStruct sharedFragment = entityManager.GetOrCreateSharedFragmentByHash<FPointoOfInterestsSharedFragment>(paramsHash, PointOfInterestsParams);
	buildContext.AddSharedFragment(sharedFragment);*/
}
