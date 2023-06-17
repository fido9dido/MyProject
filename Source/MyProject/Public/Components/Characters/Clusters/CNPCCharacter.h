// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

#include "Actors/IActorPoolable.h"
#include "Actors/IClusterActor.h"

#include "Characters/Shared/CDefaultCharacter.h"
#include "Characters/Clusters/ClusterCharacterDefinition.h"
#include "Characters/Clusters/CClusterDefinition.h"
#include "MassEntityManager.h"

#include "CNPCCharacter.generated.h"

enum class ESyncOption : uint8;

UCLASS(DefaultToInstanced)
class MYPROJECT_API ACNPCCharacter : public ACDefaultCharacter, public IClusterActor, public IActorPoolable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", Interp)
	TObjectPtr<UCClusterDefinition> ClusterCharacterData;

	// Whether to asynchronously load any assets which have not previously been loaded
	UPROPERTY(EditAnywhere, Category = "Character", Interp)
	bool bShouldAsyncLoad = true;

	UPROPERTY(EditAnywhere, Category = "Character", meta = (DisplayName = "Build Character on Contruction"))
	bool bShouldBuildOnConstruct = true;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMassAgentComponent> AgentComponent;

private:

	UPROPERTY()
	FClusterCharacterDefinition CharacterDefinition;
public:

	ACNPCCharacter(const FObjectInitializer& objectInitializer);

	// Uses the currently set options for the actor to build the corresponding character
	UFUNCTION(BlueprintCallable, Category = "Cluster")
	void BuildCharacter();

	UFUNCTION(BlueprintCallable, Category = "Cluster")
	void BuildCharacterFromDefinition(const FClusterCharacterDefinition& inCharacterDefinition);

	UFUNCTION(BlueprintCallable, Category = "Character")
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	UFUNCTION(BlueprintPure, Category = "Character")
	UDataAsset* GetCurrentLocomotionAnimSet() const;

	virtual void OnGetOrSpawn(FMassEntityManager* entitySubsystem, const FMassEntityHandle massAgent) override;

protected:
	virtual void PostInitProperties() override;
	virtual void OnConstruction(const FTransform& transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

	virtual void PreSave(FObjectPreSaveContext objectSaveContext) override;

private:

	void SetupSkeletalMeshes();
	void UpdateMeshes(const FClusterCharacterDefinition& characterDefinition);
	void UpdateSkeletalMesh(USkeletalMeshComponent* skeletalMeshComponent, TSoftObjectPtr<USkeletalMesh> softSkeletalMeshPtr);

};