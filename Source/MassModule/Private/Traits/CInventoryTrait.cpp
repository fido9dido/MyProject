// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CInventoryTrait.h"
#include "Fragments/SmartObjectQueryResultFragment.h"
#include <MassEntityTemplateRegistry.h>

void UCInventoryTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment(FConstStructView::Make(InventoryFragment));
	
}