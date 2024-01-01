// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CResourceDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALSMODULE_API UCResourceDataAsset : public UDataAsset
{
	GENERATED_BODY()	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UObject> Image;

	UPROPERTY(EditAnywhere)
	FText Name;	 
	
	UPROPERTY(EditAnywhere)
	FText Description;

}; 

UCLASS()
class GLOBALSMODULE_API UCResourceData : public UDataAsset
{
	GENERATED_BODY()	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UObject> Image;

	UPROPERTY(EditAnywhere)
	FText Name;	 
	
	UPROPERTY(EditAnywhere)
	FText Description;

};

UCLASS(DefaultToInstanced, EditInlineNew)
class GLOBALSMODULE_API UCResourceCost : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCResourceDataAsset> ResourceData;

	UPROPERTY(EditAnywhere)
	int32 Count;
};
