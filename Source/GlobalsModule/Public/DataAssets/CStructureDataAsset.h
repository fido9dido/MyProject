// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CResourceDataAsset.h"
#include "MassEntityConfigAsset.h"
#include "CStructureDataAsset.generated.h"

UCLASS()
class GLOBALSMODULE_API UCStructureDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:	

	/**
	 * The image to render for this brush, can be a UTexture or UMaterialInterface or an object implementing
	 * the AtlasedTextureInterface.
	 */
	UPROPERTY(EditAnywhere, meta = (DisplayThumbnail = "true", AllowedClasses = "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TObjectPtr<UObject> Image;

	UPROPERTY(EditAnywhere)
	FText Name;	  	
	
	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMassEntityConfigAsset> EntityConfig = nullptr;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UCResourceCost>> ResourceCostList;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMesh> StaticMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> OptionalMaterial;

public:
	bool IsValid() 
	{
		ensureMsgf(StaticMesh && EntityConfig, TEXT("Structure dataAsset is missing a mesh or config asset"));
		return StaticMesh && EntityConfig;
	}
};
