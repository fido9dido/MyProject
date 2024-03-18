// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CPreviewActor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <ConstraintsManager.h>
#include <Processors/CPlacementProcessor.h>
#include <Kismet/GameplayStatics.h>
#include <DataAssets/CStructureDataAsset.h>
#include <Materials/MaterialInterface.h>
#include "Subsystems/CPlacementSubsystem.h"

ACPreviewActor::ACPreviewActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetBoxExtent(BoxExtent);
}

void ACPreviewActor::BeginPlay()
{
	Super::BeginPlay();
	
	UCPlacementSubsystem* placementSubsystem = GetGameInstance()->GetSubsystem<UCPlacementSubsystem>();

	if (ensure(placementSubsystem))
	{
		OnPreviewEnabled.AddUObject(this, &ACPreviewActor::OnEnabled);
		OnPreviewDisabled.AddUObject(this, &ACPreviewActor::OnDisabled);
	}	
}

void ACPreviewActor::SetStaticMesh(UStaticMesh* staticMesh)
{
	MeshComponent->SetStaticMesh(staticMesh);
}	 

void ACPreviewActor::SetMaterial(const int32 index, UMaterialInterface* material)
{
	MeshComponent->SetMaterial(index, material);
}
			   
void ACPreviewActor::SetActive(bool value)
{
	SetActorTickEnabled(value);
	SetActorHiddenInGame(!value);
	SetActorEnableCollision(value);
	bActive = value;

}

void ACPreviewActor::SetStructureData(TObjectPtr<UCStructureDataAsset> structureData)
{
	StructureData = structureData;
}

bool ACPreviewActor::IsStructureDataValid()
{
	return StructureData->IsValid();
}

void ACPreviewActor::OnEnabled()
{
	if (!StructureData || !StructureData->IsValid())
	{
		return;
	}

	SetStaticMesh(StructureData->StaticMesh);
	SetMaterial(0, StructureData->OptionalMaterial);

	SetActive(true);
}
void ACPreviewActor::OnDisabled()
{
	SetActive(false);
	SetSpawnable(false);
	
	StructureData = nullptr;
}

void ACPreviewActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		bSpawnable = true;
	}
}
