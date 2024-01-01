// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControlRig.h"
#include "Rigs/RigHierarchyController.h"

#include "CControlRig.generated.h"

/**
 * 	Ignored for now for some reason reparenting is locked & am avoiding engine modification for now
 */
UCLASS(Blueprintable)
class MASSMODULE_API UCControlRig : public UControlRig
{
	GENERATED_UCLASS_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	FRigElementKey RigElementKey;

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	bool bStopTrace = true;
	
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	FVector TranslateValue = FVector(0, 5, 0);

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	float Radius = 5.f;

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	FVector TraceOffsetAbove = FVector(0.f, 0.f, 50.f);
	
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	FVector TraceOffsetBelow = FVector(0.f, 0.f, 50.f);

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Trace"))
	TEnumAsByte<ECollisionChannel> Channel;

public:
	FVector FootTrace(FRigElementKey& IKFootBone);

};
