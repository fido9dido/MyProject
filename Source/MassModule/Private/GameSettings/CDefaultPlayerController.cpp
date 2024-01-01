// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/CDefaultPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/CPreviewActor.h"
#include "UI/CHUD.h"

ACDefaultPlayerController::ACDefaultPlayerController()
{
	DefaultMouseCursor = EMouseCursor::Default; 
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACDefaultPlayerController::SetInteractingStatus(bool status)
{
	bInteracting = status;
}

void ACDefaultPlayerController::BindPreviewActor(ACPreviewActor* previewActor)
{
	if(previewActor)
	{
		previewActor->OnPreviewEnabled.AddUObject(this, &ACDefaultPlayerController::OnPreviewActorEnabled);
		previewActor->OnPreviewDisabled.AddUObject(this, &ACDefaultPlayerController::OnPreviewActorDisabled);
	}
	
}

void ACDefaultPlayerController::OnPreviewActorEnabled()
{
	SetInputMode(FInputModeGameOnly());
	SetInteractingStatus(true);
}

void ACDefaultPlayerController::OnPreviewActorDisabled()
{
	SetInputMode(FInputModeGameAndUI());
	SetInteractingStatus(false);
}