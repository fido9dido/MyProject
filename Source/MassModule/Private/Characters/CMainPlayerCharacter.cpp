// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CMainPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Characters/CCharacterMovementComponent.h>

ACMainPlayerCharacter::ACMainPlayerCharacter(const FObjectInitializer& objectInitializer)
	: ACDefaultCharacter(objectInitializer)
{

	//TODO: Set to false
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainPlayerCameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = ArmLength;
	CameraArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainPlayerCamera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

}
