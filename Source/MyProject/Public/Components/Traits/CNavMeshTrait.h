// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MassEntityTraitBase.h>
#include "Fragments/NavMeshFragment.h"
#include "CNavMeshTrait.generated.h"

UCLASS()
class MYPROJECT_API UCNavMeshTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPointoOfInterestsSharedFragment PointOfInterests;
};
