// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Animation/CControlRig.h"
#include <CharacterMovementComponentAsync.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(CControlRig)

UCControlRig::UCControlRig(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bCopyHierarchyBeforeConstruction = false;
	bResetInitialTransformsBeforeConstruction = false;
}

FVector UCControlRig::FootTrace(FRigElementKey& IKFootBone)
{
	/*FTransform transform = Hierarchy->GetGlobalTransform(CachedIndex);
	transform.AddToTranslation();
	
	FCollisionQueryParams queryParams;
	queryParams.bTraceComplex = true;
	if (ExecuteContext.GetOwningActor())
	{
		queryParams.AddIgnoredActor(ExecuteContext.GetOwningActor());
	}
	else if (const UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ExecuteContext.GetOwningComponent()))
	{
		queryParams.AddIgnoredComponent(PrimitiveComponent);
	}

	FCollisionResponseParams responseParams(ECR_Ignore);
	responseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
										
	FVector start = TranslateValue + transform.GetTranslation();
	start += TraceOffsetAbove;
	
	FVector end = TranslateValue + transform.GetTranslation();
	end += TraceOffsetBelow;

	FHitResult hitResult;
	GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, Channel, FCollisionShape::MakeSphere(Radius), queryParams, responseParams);
	return ExecuteContext.ToVMSpace(hitResult.ImpactPoint);		   */
	return FVector();
}
