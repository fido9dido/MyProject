// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MassEntityTraitBase.h>
#include "Fragments/PreviewSharedFragment.h"

#include "CPreviewTrait.generated.h"

UCLASS()
class MASSMODULE_API UCPreviewTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
	UPROPERTY(EditAnywhere)
	FPreviewSharedFragment PreviewFragment;
};
