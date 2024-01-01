// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartObjects/CGatherResourceDefinition.h"
#include "MassCommandBuffer.h"
#include "MassSmartObjectFragments.h"
#include "Tags/GatherResourceTag.h"

void UCGatherResourceDefinition::Activate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Activate(CommandBuffer, EntityContext);
}

void UCGatherResourceDefinition::Deactivate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Deactivate(CommandBuffer, EntityContext);

	const FMassEntityHandle & entityHandle = EntityContext.EntityView.GetEntity();
	CommandBuffer.AddTag<FGatherResourceTag>(entityHandle);

}
