// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CPreviewTrait.h"
#include "Fragments/SmartObjectQueryResultFragment.h"
#include <MassEntityTemplateRegistry.h>

void UCPreviewTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& entityManager = UE::Mass::Utils::GetEntityManagerChecked(World);
	uint32 paramsHash = UE::StructUtils::GetStructCrc32(FConstStructView::Make(PreviewFragment));
	FSharedStruct sharedFragment = entityManager.GetOrCreateSharedFragmentByHash<FPreviewSharedFragment>(paramsHash, PreviewFragment);
	BuildContext.AddSharedFragment(sharedFragment);

}
