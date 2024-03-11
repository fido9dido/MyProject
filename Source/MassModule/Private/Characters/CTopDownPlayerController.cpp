// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CTopDownPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/CPlacementSubsystem.h"
#include <Subsystems/CUIManagerSubsystem.h>
#include "UI/Widgets/CHUDWidget.h"

ACTopDownPlayerController::ACTopDownPlayerController()
{
	bShowMouseCursor = true;

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f; 
}

void ACTopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ACTopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ACTopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ACTopDownPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ACTopDownPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ACTopDownPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ACTopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ACTopDownPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ACTopDownPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ACTopDownPlayerController::OnTouchReleased);
		
		// Setup keyboard input events
		EnhancedInputComponent->BindAction(PlacementClickAction, ETriggerEvent::Started, this, &ACTopDownPlayerController::OnPlacementTriggered);
		
	}
	else
	{
		UE_LOG(LogTopDownCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACTopDownPlayerController::OnInputStarted()
{
	if (CurrentState!=ECPlayerState::Default) { return; }
	StopMovement();
}

// Triggered every frame when the input is held down
void ACTopDownPlayerController::OnSetDestinationTriggered()
{
	if (CurrentState != ECPlayerState::Default) { return; }
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ACTopDownPlayerController::OnSetDestinationReleased()
{
	if (CurrentState != ECPlayerState::Default) { return; }
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ACTopDownPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ACTopDownPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ACTopDownPlayerController::OnPlacementTriggered()
{
	UCPlacementSubsystem* placementSubsystem = GetGameInstance()->GetSubsystem<UCPlacementSubsystem>();
	UCUIManagerSubsystem* uiManagerSubsystem = GetGameInstance()->GetSubsystem<UCUIManagerSubsystem>();
	
	if (!placementSubsystem || !uiManagerSubsystem) { return; }
	TObjectPtr<UCommonActivatableWidget> widget = uiManagerSubsystem->FindPlaceholderWidget(StructureListPlaceholderTag);

	if (placementSubsystem->IsEnabled())
	{
		placementSubsystem->DisablePlacement();
		
		if (ensureMsgf(widget, TEXT("Placeholder Widget Doesn't exist")))
		{
			widget->DeactivateWidget();
		}
	}
	else 
	{
		placementSubsystem->EnablePlacement();
		
		if (ensureMsgf(widget, TEXT("Placeholder Widget Doesn't exist")))
		{
			widget->ActivateWidget();
		}
	}
}
