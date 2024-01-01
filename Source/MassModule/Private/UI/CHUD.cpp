// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CHUD.h"
#include "UI/Widgets/CHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/GameFrameworkComponentManager.h"
#include "DataAssets/CStructureDataAsset.h"
#include "UI/Widgets/CStructureListWidget.h"	

#include UE_INLINE_GENERATED_CPP_BY_NAME(CHUD)

ACHUD::ACHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ACHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void ACHUD::GetDebugActorList(TArray<AActor*>& InOutList)
{
}
