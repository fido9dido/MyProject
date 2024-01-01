// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartObjects/CConstructionBehaviourDefinition.h"
#include "MassCommandBuffer.h"
#include "MassEntityTypes.h"
#include "Tags/StructureTags.h"

void UCConstructionBehaviourDefinition::Activate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Activate(CommandBuffer, EntityContext);

	const FMassEntityHandle& entityHandle = EntityContext.EntityView.GetEntity();

	CommandBuffer.AddTag<FConstructionBeginTag>(entityHandle);
}

void UCConstructionBehaviourDefinition::Deactivate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Deactivate(CommandBuffer, EntityContext);
}
