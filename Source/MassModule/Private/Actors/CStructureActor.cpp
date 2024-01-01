// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CStructureActor.h"
#include <Subsystems/CUIManagerSubsystem.h>
#include <Subsystems/CPlacementSubsystem.h>
#include "CommonActivatableWidget.h"
#include "UI/Widgets/CStructureInfoWidget.h"
#include "LogTypes.h"
#include "DataAssets/CStructureDataAsset.h"
#include "SmartObjectComponent.h"

ACStructureActor::ACStructureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ACStructureActor::OnGetOrSpawn(struct FMassEntityManager& entityManager, const FMassEntityHandle entityHandle)
{
	StructureHandle = entityHandle;

	UCPlacementSubsystem* PlacementSubsystem = GetGameInstance()->GetSubsystem<UCPlacementSubsystem>();
	ensure(PlacementSubsystem->SpawnStructureDataMap.Contains(entityHandle));
	StructureData = PlacementSubsystem->SpawnStructureDataMap.FindAndRemoveChecked(entityHandle);
	MeshComponent->SetStaticMesh(StructureData->StaticMesh);
	
	if (StructureData->OptionalMaterial)
	{
		MeshComponent->SetMaterial(0, StructureData->OptionalMaterial);
	}
}


void ACStructureActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked();

	if (!ensure(PlaceholderTag.IsValid()))
	{
		UE_LOG(LogCUI, Log, TEXT("CStructureActor: [%s] PlaceholderTag is not valid"), *GetName());

		return;
	}

	if (!ensure(StructureHandle.IsValid()))
	{
		UE_LOG(LogCUI, Log, TEXT("CStructureActor: [%s] StructureHandle is not valid"), *GetName());

		return;
	}		  
	if (!ensure(StructureData.Get()))
	{
		UE_LOG(LogCUI, Log, TEXT("CStructureActor: [%s] StructureData is not valid"), *GetName());

		return;
	}

	OnStructureClicked.Broadcast(StructureHandle, StructureData.Get());

	if (StructureInfoWidget)
	{
		StructureInfoWidget->ActivateWidget();
	}
}

void ACStructureActor::PreSave(FObjectPreSaveContext objectSaveContext)
{
	if (ensure(PlaceholderTag.IsValid()))
	{
		UE_LOG(LogCUI, Log, TEXT("CStructureActor: [%s] Doesn't have a valid Placeholder Tag"), *GetName());
		return;
	}
	PreSave(objectSaveContext);
}

void ACStructureActor::BeginPlay()
{
	Super::BeginPlay();
	UCUIManagerSubsystem* uiManagerSubsystem = GetGameInstance()->GetSubsystem<UCUIManagerSubsystem>();
	TObjectPtr<UCommonActivatableWidget>* structureInfoWidgetPtr = uiManagerSubsystem->GetPlaceholderWidget(PlaceholderTag);
	if (structureInfoWidgetPtr)
	{
		StructureInfoWidget = Cast<UCStructureInfoWidget>(*structureInfoWidgetPtr);
		OnStructureClicked.AddDynamic(StructureInfoWidget, &UCStructureInfoWidget::OnClicked);
	}
}
