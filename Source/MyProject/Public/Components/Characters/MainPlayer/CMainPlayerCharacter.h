// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Shared/CDefaultCharacter.h"
#include "CMainPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACMainPlayerCharacter : public ACDefaultCharacter
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;	
	
	UPROPERTY(VisibleAnywhere)
	float ArmLength = 300.f;

public:
	ACMainPlayerCharacter(const FObjectInitializer& objectInitializer);

};
