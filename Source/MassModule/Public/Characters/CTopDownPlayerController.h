// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Characters/CDefaultPlayerController.h"
#include "MassEntityTypes.h"
#include "LogTypes.h"
#include "CTopDownPlayerController.generated.h"

UCLASS()
class MASSMODULE_API ACTopDownPlayerController :
	public ACDefaultPlayerController
{
	GENERATED_BODY()

public:

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SetDestinationTouchAction;

	/** Placement Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PlacementClickAction;
protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

private:
	UPROPERTY(VisibleAnywhere)
	FVector CachedDestination;

	UPROPERTY(VisibleAnywhere)
	bool bIsTouch; // Is it a touch device

	UPROPERTY(VisibleAnywhere)
	float FollowTime; // For how long it has been pressed

public:
	ACTopDownPlayerController();

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void OnPlacementTriggered();
};
