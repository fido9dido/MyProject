// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "InputAction.h"

#include "IControlsOverlay.generated.h"

/**
 * Interface for getting a controls description.
 * @see UControlsOverlay
 */
UINTERFACE(BlueprintType)
class MASSMODULE_API UControlsOverlay : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for getting a controls description.
 * @see UControlsOverlay
 */
class MASSMODULE_API IControlsOverlay : public IInterface
{
	GENERATED_BODY()

public:
	/** 
	 *	Hook for derived classes to provide a description of the action 
	 *	that should be associated with a given input action.
	 *
	 *	@returns InputAction-String mappings that represent a control description.
	 * 
	 *	@note	The UI component should default empty descriptions to the 
	 *			name of the input action when updating the controls overlay.
	 *
	 *	@see UUIComponent::UpdateControlsOverlay, UControlsOverlay
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Controls Overlay")
	TMap<UInputAction*, FText> GetInputActionDescriptions() const;

	/** 
	 *	Hook for derived classes to provide a description of the action 
	 *	that should be associated with a given input key.
	 *
	 *	@returns Key-String mappings that represent a control description.
	 * 
	 *	@note	Key description overrides take priority over input action descriptions.
	 *
	 *	@see UUIComponent::UpdateControlsOverlay, UControlsOverlay
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Controls Overlay")
	TMap<FKey, FText> GetInputKeyDescriptionOverrides() const;
};