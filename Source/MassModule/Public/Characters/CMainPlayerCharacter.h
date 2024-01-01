// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CDefaultCharacter.h"
#include "CMainPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API ACMainPlayerCharacter : public ACDefaultCharacter
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;	
	
	UPROPERTY(VisibleAnywhere)
	float ArmLength = 300.f; //or 800.0

public:
	ACMainPlayerCharacter(const FObjectInitializer& objectInitializer);

};
