// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/MainPlayer/CMainPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Characters/Shared/CCharacterMovementComponent.h>

ACMainPlayerCharacter::ACMainPlayerCharacter(const FObjectInitializer& objectInitializer)
	: ACDefaultCharacter(objectInitializer.SetDefaultSubobjectClass<UCCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//TODO: Set to false
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainPlayerCameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = ArmLength;
	CameraArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainPlayerCamera"));
	Camera->SetupAttachment(CameraArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}
