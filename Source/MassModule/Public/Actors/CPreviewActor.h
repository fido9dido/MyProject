// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "CPreviewActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPreviewEnabled);
DECLARE_MULTICAST_DELEGATE(FOnPreviewDisabled);

UCLASS()
class MASSMODULE_API ACPreviewActor : public AActor
{
	GENERATED_BODY()

public:		

	FOnPreviewEnabled OnPreviewEnabled;
	FOnPreviewDisabled OnPreviewDisabled;

	bool bActive = false;
	bool bSpawnable = false;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	float Radius = 90.f;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	FVector BoxExtent = FVector(64.f);

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxComponent = nullptr;

	TWeakObjectPtr<class UCStructureDataAsset>	StructureData = nullptr;

public:
	ACPreviewActor();

	/** Event when this actor is clicked by the mouse when using the clickable interface. */
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;

	void SetStaticMesh(class UStaticMesh* staticMesh);
	void SetMaterial(const int32 index, class UMaterialInterface* material);
	void SetActive(bool value);
	void SetStructureData(TWeakObjectPtr<class UCStructureDataAsset> structureData);
	bool IsStructureDataValid() { return StructureData.IsValid(); }
	void SetSpawnable(bool value) { bSpawnable = value; }
	bool IsSpawnable() { return bSpawnable; }
	bool IsActive() { return bActive; }
	void OnEnabled();
	void OnDisabled();

protected:
	virtual void BeginPlay() override;

};