// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CDefaultPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/CPreviewActor.h"
#include "UI/CHUD.h"
#include "Subsystems/CPlacementSubsystem.h"


ACDefaultPlayerController::ACDefaultPlayerController()
{
	DefaultMouseCursor = EMouseCursor::Default; 
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACDefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UCPlacementSubsystem* placementSubsystem = GetGameInstance()->GetSubsystem<UCPlacementSubsystem>();

	if (ensure(placementSubsystem))
	{
		placementSubsystem->OnPlacementEnabled.AddUObject(this, &ACDefaultPlayerController::OnPlacementEnabled);
		placementSubsystem->OnPlacementDisabled.AddUObject(this, &ACDefaultPlayerController::OnPlacementDisabled);
	}
}

void ACDefaultPlayerController::SetPlayerState(ECPlayerState newState)
{
	CurrentState = newState;
}

void ACDefaultPlayerController::InputModeGameOnly()
{
	SetInputMode(FInputModeGameOnly());
} 

void ACDefaultPlayerController::InputModeDefault()
{
	SetInputMode(FInputModeGameAndUI());
}

void ACDefaultPlayerController::OnPlacementEnabled()
{
	SetPlayerState(ECPlayerState::Placement);
}

void ACDefaultPlayerController::OnPlacementDisabled()
{
	SetPlayerState(ECPlayerState::Default);
}