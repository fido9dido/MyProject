// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubSystem.h"
#include <MassEntityTypes.h>
#include "CPlacementSubsystem.generated.h"

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

	TMap<FMassEntityHandle, TObjectPtr<const UCStructureDataAsset>> SpawnStructureDataMap;

private:

	UPROPERTY(EditAnywhere, Category = "Preview", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMassEntityConfigAsset> PreviewConfig;

	UPROPERTY(VisibleAnywhere, Category = "Preview")
	FMassEntityHandle PreviewHandle;

	TObjectPtr<class ACPreviewActor> PreviewActor = nullptr;

public:

	FMassEntityHandle& GetPreviewHandle();
	class UMassEntityConfigAsset* GetPreviewConfig();
	void SetPreviewHandle(const FMassEntityHandle& previewHandle);
	
	//~ Begin USubsystem implementation 
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	//~ End USubsystem implementation 

	void SpawnPreviewEntity(UWorld& world);
	void OnBeginPlay(UWorld& world);

	class ACPreviewActor* GetPreviewActor() { return PreviewActor; }
	void SetPreviewActor(ACPreviewActor* previewActor);

};