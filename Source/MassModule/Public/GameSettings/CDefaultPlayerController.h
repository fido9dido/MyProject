// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Controls/IControlsOverlay.h"
#include "Controls/IInput.h"

#include "CDefaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API ACDefaultPlayerController
	: public APlayerController
	, public IControlsOverlay
	, public IInput
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	bool bInteracting = false;
private:
public:
	ACDefaultPlayerController();

	void SetInteractingStatus(bool status);

	void BindPreviewActor(class ACPreviewActor* previewActor);

	UFUNCTION()
	void OnPreviewActorEnabled();
	
	UFUNCTION()
	void OnPreviewActorDisabled();

};
