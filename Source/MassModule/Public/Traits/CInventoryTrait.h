// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MassEntityTraitBase.h>
#include "Fragments/InventoryFragment.h"

#include "CInventoryTrait.generated.h"

UCLASS()
class MASSMODULE_API UCInventoryTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
	UPROPERTY(EditAnywhere)
	FInventoryFragment InventoryFragment;
};
