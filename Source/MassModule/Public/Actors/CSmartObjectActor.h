// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "CSmartObjectActor.generated.h"

UCLASS()
class ACSmartObjectActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class UContextualAnimSceneActorComponent* ContextualAnimSceneActorComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class USmartObjectComponent* SmartObjectComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class UNavModifierComponent* NavModifierComponent = nullptr;

public:
	ACSmartObjectActor();

	struct FSmartObjectHandle GetRegisteredHandle();

};