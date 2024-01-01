// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

#include "Actors/IActorPoolable.h"
#include "Actors/ICustomRepresentationActor.h"

#include "Characters/CDefaultCharacter.h"
#include "Characters/ClusterDefinitions/ClusterCharacterDefinition.h"
#include "Characters/ClusterDefinitions/CClusterDefinitionDataAsset.h"
#include "MassEntityManager.h"

#include "CAgentCharacter.generated.h"

enum class ESyncOption : uint8;

UCLASS(DefaultToInstanced)
class MASSMODULE_API ACAgentCharacter : public ACDefaultCharacter, public ICustomRepresentationActor, public IActorPoolable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMassAgentComponent> AgentComponent;

public:

	ACAgentCharacter(const FObjectInitializer& objectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void BuildCharacterFromDefinition(const FClusterCharacterDefinition& characterDefinitionx);

	UFUNCTION(BlueprintCallable, Category = "Character")
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	virtual void OnGetOrSpawn(FMassEntityManager& entityManager, const FMassEntityHandle massAgent) override;

protected:
	virtual void PostInitProperties() override;
	virtual void PreSave(FObjectPreSaveContext objectSaveContext) override;

private:

	void SetupSkeletalMeshes();
	void SetSkeletalMesh(const FClusterCharacterDefinition& characterDefinition);
	void UpdateSkeletalMesh(USkeletalMeshComponent* skeletalMeshComponent, TSoftObjectPtr<USkeletalMesh> softSkeletalMeshPtr);
};