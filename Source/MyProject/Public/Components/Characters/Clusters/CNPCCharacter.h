// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

#include "Actors/IActorPoolable.h"
#include "Actors/IClusterActor.h"

#include "Characters/Shared/CDefaultCharacter.h"
#include "Characters/Clusters/ClusterCharacterDefinition.h"
#include "Characters/Clusters/CClusterDefinitionDataAsset.h"
#include "MassEntityManager.h"

#include "CNPCCharacter.generated.h"

enum class ESyncOption : uint8;

UCLASS(DefaultToInstanced)
class MYPROJECT_API ACNPCCharacter : public ACDefaultCharacter, public IClusterActor, public IActorPoolable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMassAgentComponent> AgentComponent;

public:

	ACNPCCharacter(const FObjectInitializer& objectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void BuildCharacterFromDefinition(const FClusterCharacterDefinition& characterDefinitionx);

	UFUNCTION(BlueprintCallable, Category = "Character")
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	virtual void OnGetOrSpawn(FMassEntityManager* entitySubsystem, const FMassEntityHandle massAgent) override;

protected:
	virtual void PostInitProperties() override;
	virtual void PreSave(FObjectPreSaveContext objectSaveContext) override;

private:

	void SetupSkeletalMeshes();
	void SetSkeletalMesh(const FClusterCharacterDefinition& characterDefinition);
	void UpdateSkeletalMesh(USkeletalMeshComponent* skeletalMeshComponent, TSoftObjectPtr<USkeletalMesh> softSkeletalMeshPtr);
};