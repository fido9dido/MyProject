// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "CGameSettings.generated.h"

class UMassEntityConfigAsset;

UCLASS(config=Game, defaultconfig, meta=(DisplayName="CGameSettings"))
class MASSMODULE_API UCGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category = "Placement.Structure", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<class AActor> PreviewActorClass = nullptr;

	UPROPERTY(config, EditAnywhere, Category = "Placement.Structure")
	TSoftObjectPtr<UMassEntityConfigAsset> PreviewConfigAsset;
	
	UPROPERTY(config, EditAnywhere, Category = "Worker")
	TSoftObjectPtr<UMassEntityConfigAsset> WorkerEntityAsset;

public:

	static UCGameSettings* GetMutable();
	static const UCGameSettings* Get();
	
	class UMassEntityConfigAsset* GetPreviewConfig() const;

	TSubclassOf<AActor> GetPreviewActorClass();
	
};
