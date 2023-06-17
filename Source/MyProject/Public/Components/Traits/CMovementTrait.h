// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "CMovementTrait.generated.h"

UCLASS()
class MYPROJECT_API UCMovementTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
};
