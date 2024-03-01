// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GlobalTypes.h>
#include <MassSmartObjectBehaviorDefinition.h>
#include "CGatherResourceDefinition.generated.h"

UCLASS()
class MASSMODULE_API UCGatherResourceDefinition : public USmartObjectMassBehaviorDefinition
{
	GENERATED_BODY()

public:
	virtual void Activate(struct FMassCommandBuffer& CommandBuffer, const struct FMassBehaviorEntityContext& EntityContext) const override;
	virtual void Deactivate(struct FMassCommandBuffer& CommandBuffer, const struct FMassBehaviorEntityContext& EntityContext) const override;

};
