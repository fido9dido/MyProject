// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Controls/Inputs/IControlsOverlay.h"
#include "Controls/Inputs/IInput.h"

#include "CDefaultCharacter.generated.h"

class UMotionWarpingComponent;

UCLASS()
class MYPROJECT_API ACDefaultCharacter : public ACharacter,
	public IControlsOverlay,
	public IInput
{
	GENERATED_BODY()

public:

	/** Mapping of pawn relevant InputActions to their string description for the controls overlay UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Overlay")
	TMap<UInputAction*, FText> InputActionDescriptions;

	/** Mapping of pawn relevant input keys to their string description for the controls overlay UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Overlay")
	TMap<FKey, FText> InputKeyDescriptionOverrides;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


private:

	/** Flag that indicates whether the character is currently sprinting */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, BlueprintSetter = SetIsSprinting, Meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	/** Speed Character attempts to reach when sprinting */
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	float DefaultSprintSpeed = 450.f;

	/** Speed Character attempts to reach when walking */
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed = 170.f;

	/** Starting input acceleration size passed to the movement component when walking */
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	float DefaultWalkMinInputSize = 0.0f;

	/** Starting input acceleration size passed to the movement component when sprinting */
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	float DefaultSprintMinInputSize = 0.5f;

	/** Input Mapping and Input Actions that Characters depend on */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* LookControlsInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookDeltaAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 InputMappingPriority = 1;

public:
	ACDefaultCharacter(const FObjectInitializer& objectInitializer);

	virtual TMap<UInputAction*, FText> GetInputActionDescriptions_Implementation() const override
	{
		return InputActionDescriptions;
	};

	virtual TMap<FKey, FText> GetInputKeyDescriptionOverrides_Implementation() const override
	{
		return InputKeyDescriptionOverrides;
	};
	
	virtual void AddInputContext_Implementation(const TScriptInterface<IEnhancedInputSubsystemInterface>& SubsystemInterface) override;
	virtual void RemoveInputContext_Implementation(const TScriptInterface<IEnhancedInputSubsystemInterface>& SubsystemInterface) override;

	UFUNCTION(BlueprintPure)
	bool GetIsSprinting() const
	{
		return bIsSprinting;
	}

	/** Sets sprinting flag and calls OnIsSprintingChanged BIE if valid */
	UFUNCTION(BlueprintCallable)
	void SetIsSprinting(bool bNewIsSprinting);

	UFUNCTION(BlueprintPure)
	float GetDesiredWalkSpeed() const
	{
		return DefaultWalkSpeed;
	}

	UFUNCTION(BlueprintPure)
	float GetDesiredSprintSpeed() const
	{
		return DefaultSprintSpeed;
	}

	UFUNCTION(BlueprintPure)
	float GetDesiredWalkMinInputSize() const
	{
		return DefaultWalkMinInputSize;
	}

	UFUNCTION(BlueprintPure)
	float GetDesiredSprintMinInputSize() const
	{
		return DefaultSprintMinInputSize;
	}


protected:

	//~ Begin APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End APawn interface

	/* Allow cheat flying to move vertically */
	bool ShouldLimitMovementPitch() const;

	/** BP hook for when the character's sprinting state changes */
	UFUNCTION(BlueprintImplementableEvent)
	void OnIsSprintingChanged(bool bNewIsSprinting);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Added to accommodate Gamepad 2D Axis IA support. Passes Value.X to MoveRight and Value.Y to MoveForward */
	void Move(const FVector2D& Value);

	/** Called via input to turn at a given rate. Uses a normalized rate. i.e. 1.0 means 100% of desired turn rate */
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate. Uses a normalized rate. i.e. 1.0 means 100% of desired turn rate */
	void LookUpAtRate(float Rate);

	/** Added to accommodate Gamepad 2D Axis IA support. Passes Value.X to TurnAtRate and Value.Y to LookUpAtRate */
	void Look(const FVector2D& Value);

 private:

	/** Methods binded to character input actions to facilitate player input */
	void MoveActionBinding(const struct FInputActionValue& ActionValue);
	void LookActionBinding(const struct FInputActionValue& ActionValue);
	void LookDeltaActionBinding(const struct FInputActionValue& ActionValue);
};
