// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API ACHUD : public AHUD
{
	GENERATED_BODY()


public:
	ACHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//TODO Delete the following functions
	//I don't need this class anymore for now
	
	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AHUD interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
	//~End of AHUD interface


};
