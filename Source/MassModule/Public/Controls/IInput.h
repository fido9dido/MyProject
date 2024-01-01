// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "UObject/Interface.h"
#include "EnhancedInputSubsystemInterface.h"
#include "IInput.generated.h"

template<class UserClass, typename FuncType>
static bool BindInputAction(
	UEnhancedInputComponent* PlayerInputComponent,
	const UInputAction* Action,
	const ETriggerEvent EventType,
	UserClass* Object, FuncType Func)
{
	if (ensure(Action != nullptr))
	{
		PlayerInputComponent->BindAction(Action, EventType, Object, Func);
		return true;
	}

	return false;
}

UINTERFACE(BlueprintType, Blueprintable)
class MASSMODULE_API UInput : public UInterface
{
	GENERATED_UINTERFACE_BODY()
public:

};

class MASSMODULE_API IInput : public IInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	/** Called to add any necessary inputs. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddInputContext(const TScriptInterface<IEnhancedInputSubsystemInterface>& SubsystemInterface);

	/** Called to remove any necessary inputs. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveInputContext(const TScriptInterface<IEnhancedInputSubsystemInterface>& SubsystemInterface);
};