// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "GlobalTypes.h"
#include "SmartObjectTypes.h"
#include "MassSmartObjectRequest.h"

#include "SmartObjectFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FSmartObjectFragment : public FMassFragment
{
	GENERATED_BODY()

public:
	FSmartObjectHandle RegisteredHandle;
	
};
