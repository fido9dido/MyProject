// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CResourceTrait.h"
#include "Fragments/SmartObjectQueryResultFragment.h"
#include <MassEntityTemplateRegistry.h>

void UCResourceTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment(FConstStructView::Make(ResourceFragment));
	
}
