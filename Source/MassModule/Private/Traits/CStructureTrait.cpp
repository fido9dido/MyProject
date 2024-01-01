// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CStructureTrait.h"
#include "Fragments/SmartObjectQueryResultFragment.h"
#include <MassEntityTemplateRegistry.h>
#include <MassCommonFragments.h>

void UCStructureTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment(FConstStructView::Make(StructureFragment));
	BuildContext.AddFragment<FSmartObjectQueryResultFragment>();
	BuildContext.RequireFragment<FTransformFragment>();
}
