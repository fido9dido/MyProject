// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Controls/IControlsOverlay.h"
#include "Controls/IInput.h"

#include "CMainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API ACMainPlayerController
	: public APlayerController
	, public IControlsOverlay
	, public IInput
{
	GENERATED_BODY()
public:
	
	/** Attempts to activate the input context mapping of the pawn that is passed in */
	void AddPawnInputContext(APawn* toAdd);

	/** Attempts to remove the input context mapping of the pawn that is passed in */
	void RemovePawnInputContext(APawn* toRemove);

	void MarkControlsDirty();

protected:
	virtual void BeginPlay() override;
};
