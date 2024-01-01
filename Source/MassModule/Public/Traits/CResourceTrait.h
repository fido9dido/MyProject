// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MassEntityTraitBase.h>
#include "Fragments/ResourceFragment.h"

#include "CResourceTrait.generated.h"

UCLASS()
class MASSMODULE_API UCResourceTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
	UPROPERTY(EditAnywhere)
	FResourceFragment ResourceFragment;
};
