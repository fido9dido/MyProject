// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "GlobalTypes.h"
#include "ResourceFragment.generated.h"

namespace MassSample::Signals
{
	static const FName OnResourceAcquired = FName("ResourceAcquired");
}

USTRUCT()
struct MASSMODULE_API FResourceFragment : public FMassFragment
{
	GENERATED_BODY()
	
public:	 
	UPROPERTY(EditAnywhere)
	ECResourceType ResourceType = ECResourceType::Stone;
	
};
