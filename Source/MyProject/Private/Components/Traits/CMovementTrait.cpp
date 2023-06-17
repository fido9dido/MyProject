// Fill out your copyright notice in the Description page of Project Settings.

#include "Traits/CMovementTrait.h"
#include "Fragments/MovementFragment.h"
#include <MassEntityTemplateRegistry.h>

void UCMovementTrait::BuildTemplate(FMassEntityTemplateBuildContext& buildContext, const UWorld& world) const
{			 
	buildContext.AddFragment<FMovementFragment>();
}
