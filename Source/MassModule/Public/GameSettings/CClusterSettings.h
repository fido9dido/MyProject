// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "CClusterSettings.generated.h"

class UMassEntityConfigAsset;

UCLASS(config=Mass, defaultconfig, meta=(DisplayName="Cluster"))
class MASSMODULE_API UCClusterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category = "Cluster")
	TSoftObjectPtr<UMassEntityConfigAsset> AgentConfigAsset;

private:
	UPROPERTY()
	UMassEntityConfigAsset* AgentConfig;

public:
	virtual void PostInitProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UFUNCTION(BlueprintCallable)
	static UCClusterSettings* GetMutable()
	{
		return GetMutableDefault<UCClusterSettings>();
	}

	UFUNCTION(BlueprintCallable)
	static const UCClusterSettings* Get()
	{
		return GetDefault<UCClusterSettings>();
	}

	UMassEntityConfigAsset* GetAgentConfig() const { return AgentConfig; }
	
};
