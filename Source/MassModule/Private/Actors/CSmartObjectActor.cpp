// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CSmartObjectActor.h"
#include <ContextualAnimSceneActorComponent.h>
#include <SmartObjectComponent.h>
#include <NavModifierComponent.h>
#include <Kismet/GameplayStatics.h>

ACSmartObjectActor::ACSmartObjectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent  = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	ContextualAnimSceneActorComponent = CreateDefaultSubobject<UContextualAnimSceneActorComponent>(TEXT("ContextualAnimSceneActorComponent"));
	SmartObjectComponent = CreateDefaultSubobject<USmartObjectComponent>(TEXT("SmartObjectComponent"));
	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifierComponent"));
		
	MeshComponent->SetupAttachment(RootComponent);
	ContextualAnimSceneActorComponent->SetupAttachment(MeshComponent);
	SmartObjectComponent->SetupAttachment(ContextualAnimSceneActorComponent);
}

FSmartObjectHandle ACSmartObjectActor::GetRegisteredHandle()
{
	return SmartObjectComponent->GetRegisteredHandle();
}

void ACSmartObjectActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked();
}

void ACSmartObjectActor::BeginPlay()
{
	Super::BeginPlay();
}
