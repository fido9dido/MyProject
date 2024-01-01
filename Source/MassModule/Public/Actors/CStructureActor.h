// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "CSmartObjectActor.h"
#include "Actors/ICustomRepresentationActor.h"
#include "GameplayTagContainer.h"
#include "CStructureActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnStructureClicked, ACStructureActor, OnStructureClicked, const FMassEntityHandle&, structureHandle, const UCStructureDataAsset*, structureData);

UCLASS()
class ACStructureActor : public ACSmartObjectActor, public ICustomRepresentationActor
{
	GENERATED_BODY()
	
public:
	FMassEntityHandle StructureHandle;
	FOnStructureClicked OnStructureClicked;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<const class UCStructureDataAsset> StructureData;
	
	//Tag used to reference the UI slot that displays the structure's info
	UPROPERTY(EditAnywhere)
	FGameplayTag PlaceholderTag;

	UPROPERTY(Transient)
	class UCStructureInfoWidget* StructureInfoWidget;
public:
	ACStructureActor();

	virtual void OnGetOrSpawn(struct FMassEntityManager& entityManager, const FMassEntityHandle entityHandle) override;

	/** Event when this actor is clicked by the mouse when using the clickable interface. */
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton);
protected:
	virtual void PreSave(FObjectPreSaveContext objectSaveContext) override;
	virtual void BeginPlay() override;
};