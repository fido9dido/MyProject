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

void ACPreviewActor::SetStaticMesh(UStaticMesh* staticMesh)
{
	MeshComponent->SetStaticMesh(staticMesh);
	SetActive(true);
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
	this->bActive = value;
}

void ACPreviewActor::SetStructureData(TWeakObjectPtr<UCStructureDataAsset> structureData)
{
	StructureData = structureData;
}

void ACPreviewActor::OnEnabled()
{
	if (!ensureAlwaysMsgf(StructureData.IsValid(), TEXT("Structure Data Is not assigned to PreviewActor")))
	{
		return;
	}

	SetStaticMesh(StructureData->StaticMesh);

	SetMaterial(0, StructureData->OptionalMaterial);
	
	OnPreviewEnabled.Broadcast();
}
void ACPreviewActor::OnDisabled()
{
	SetActive(false);
	SetSpawnable(false);
	
	StructureData.Reset();
	OnPreviewDisabled.Broadcast();
	
}

void ACPreviewActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		bSpawnable = true;
	}
}
