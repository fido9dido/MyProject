// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Controls/IControlsOverlay.h"
#include "Controls/IInput.h"

#include "CDefaultPlayerController.generated.h"

UENUM()
enum class ECPlayerState
{
	Default
	, Placement
	, Count

};
/**
 * 
 */
UCLASS()
class MASSMODULE_API ACDefaultPlayerController
	: public APlayerController
	, public IInput
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	ECPlayerState CurrentState = ECPlayerState::Default;
private:
protected:
	virtual void BeginPlay() override;

public:
	ACDefaultPlayerController();

	void SetPlayerState(ECPlayerState newState);
	
	UFUNCTION()
	void InputModeGameOnly();
	
	UFUNCTION()
	void InputModeDefault();

	UFUNCTION()
	void OnPlacementEnabled();

	UFUNCTION()
	void OnPlacementDisabled();

};
