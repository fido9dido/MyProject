// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassRepresentationSubsystem.h"
#include "CStructureRepresentationSubsystem.generated.h"
/**
 * Subsystem responsible for all visual of mass crowd agents, will handle actors spawning and static mesh instances
 */
UCLASS()
class MASSMODULE_API UCStructureRepresentationSubsystem : public UMassRepresentationSubsystem
{
	GENERATED_BODY()

protected:
	//~ BEGIN USubsystem 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ END USubsystem 
};