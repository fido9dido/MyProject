// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Animation/CControlRig.h"
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
	
	return FVector();
}
