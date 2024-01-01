// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "IActorPoolable.generated.h"

UINTERFACE(Blueprintable)
class MASSMODULE_API UActorPoolable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class MASSMODULE_API IActorPoolable : public IInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Mass|Actor Pooling")
	bool CanBePooled();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Mass|Actor Pooling")
	void PrepareForPooling();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Mass|Actor Pooling")
	void PrepareForGame();
};