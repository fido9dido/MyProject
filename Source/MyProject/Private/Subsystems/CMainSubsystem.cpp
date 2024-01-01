// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CMainSubsystem.h"
#include "Engine/GameInstance.h"
#include <Subsystems/CPlacementSubsystem.h>
#include <Subsystems/CUIManagerSubsystem.h>
#include <Subsystems/CWorkerSubsystem.h>

void UCMainSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	UGameInstance* gameInstance = InWorld.GetGameInstance();
	
	UCUIManagerSubsystem* uiSubsystem = gameInstance->GetSubsystem<UCUIManagerSubsystem>();

	if (uiSubsystem)
	{
		uiSubsystem->OnBeginPlay(InWorld);
	}

	UCPlacementSubsystem* placementSubsystem = gameInstance->GetSubsystem<UCPlacementSubsystem>();
	if (placementSubsystem)
	{
		placementSubsystem->OnBeginPlay(InWorld);
	}		 	

	UCWorkerSubsystem* workerSubsystem = gameInstance->GetSubsystem<UCWorkerSubsystem>();
	if (workerSubsystem)
	{
		workerSubsystem->OnBeginPlay(InWorld);
	}


}

TStatId UCMainSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCMainSubsystem, STATGROUP_Tickables);
}
