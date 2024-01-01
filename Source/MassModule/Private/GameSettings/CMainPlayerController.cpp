// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/CMainPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ACMainPlayerController::AddPawnInputContext(APawn* toAdd)
{
	if (!toAdd) { return; }
	if (ULocalPlayer* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* playerSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			const TScriptInterface<IEnhancedInputSubsystemInterface> enhancedInputInterface = playerSubsystem;
			if (toAdd->GetClass()->ImplementsInterface(UInput::StaticClass()))
			{
				IInput::Execute_AddInputContext(toAdd, enhancedInputInterface);
				MarkControlsDirty();
			}
		}
	}
}

void ACMainPlayerController::RemovePawnInputContext(APawn* toRemove)
{
	if (!toRemove) { return; }
	if (ULocalPlayer* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* playerSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			const TScriptInterface<IEnhancedInputSubsystemInterface> enhancedInputInterface = playerSubsystem;
			if (toRemove->GetClass()->ImplementsInterface(UInput::StaticClass()))
			{
				IInput::Execute_RemoveInputContext(toRemove, enhancedInputInterface);
				MarkControlsDirty();
			}
		}
	}
}

void ACMainPlayerController::MarkControlsDirty()
{
	/*if (IsValid(UIComponent))
	{
		UIComponent->RequestControlsOverlayUpdate();
	}*/
}

void ACMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* possessedPawn = GetPawn())
	{
		AddPawnInputContext(possessedPawn);
	}
}
