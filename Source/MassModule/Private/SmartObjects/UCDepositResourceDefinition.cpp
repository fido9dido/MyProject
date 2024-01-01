// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartObjects/CDepositResourceDefinition.h"
#include "MassCommandBuffer.h"
#include "Tags/DepositResourceTag.h"


void UCDepositResourceDefinition::Activate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Activate(CommandBuffer, EntityContext);
}

void UCDepositResourceDefinition::Deactivate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Deactivate(CommandBuffer, EntityContext);

	const FMassEntityHandle & entityHandle = EntityContext.EntityView.GetEntity();
	CommandBuffer.AddTag<FDepositResourceTag>(entityHandle);
}
