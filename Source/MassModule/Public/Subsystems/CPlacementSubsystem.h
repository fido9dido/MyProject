// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubSystem.h"
#include <MassEntityTypes.h>
#include "CPlacementSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlacementEnabled);
DECLARE_MULTICAST_DELEGATE(FOnPlacementDisabled);

UCLASS()
class MASSMODULE_API UCSpawnStructureData : public UObject
{
	GENERATED_BODY()
public:
	FMassEntityHandle StructureHandle; 
	class UCStructureDataAsset* StructureData;
};

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCPlacementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	FOnPlacementEnabled OnPlacementEnabled;

	FOnPlacementDisabled OnPlacementDisabled;

private:

	UPROPERTY(EditAnywhere, Category = "Preview", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMassEntityConfigAsset> PreviewConfig;

	UPROPERTY(VisibleAnywhere, Category = "Preview")
	FMassEntityHandle PreviewHandle;

	TObjectPtr<class ACPreviewActor> PreviewActor = nullptr;

	bool bEnabled = false;
	
	//Spawn
	//map that tempraily hold structure data until it is passed to the corresponding actor
	TMap<FMassEntityHandle, TObjectPtr<const UCStructureDataAsset>> SpawnedStructureDataMap;

public:

	bool  IsEnabled() { return bEnabled; }
	class ACPreviewActor* GetPreviewActor() { return PreviewActor.Get(); }
	void SetPreviewActor(ACPreviewActor* previewActor);
	FMassEntityHandle& GetPreviewHandle();
	class UMassEntityConfigAsset* GetPreviewConfig();
	void SetPreviewHandle(const FMassEntityHandle& previewHandle);	
	void AddStructure(const FMassEntityHandle& structureHandle, const class UCStructureDataAsset* structureData);
	void SpawnPreviewEntity(UWorld& world);
	void OnBeginPlay(UWorld& world);
	void EnablePlacement();
	void DisablePlacement();
	void InitalizePreviewActor(UCStructureDataAsset* structureData);
	// Spawn 	
	TMap<FMassEntityHandle, TObjectPtr<const UCStructureDataAsset>>& GetSpawnedStructureDataMap() { return SpawnedStructureDataMap; }
	bool SpawnedStructureExists(const FMassEntityHandle& structureHandle);

protected:
	//~ Begin USubsystem implementation 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End USubsystem implementation 
};