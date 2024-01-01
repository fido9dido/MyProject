// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassSettings.h"
#include "CClusterAnimationSettings.generated.h"

UCLASS(config = Mass, defaultconfig, meta=(DisplayName="Cluster Animation"))
class MASSMODULE_API UCClusterAnimationSettings : public UMassModuleSettings
{
	GENERATED_BODY()

public:

	static const UCClusterAnimationSettings* Get()
	{
		return GetDefault<UCClusterAnimationSettings>();
	}

	UPROPERTY(EditAnywhere, config, Category = LOD);
	TArray<int32> CrowdAnimFootLODTraceFrequencyPerLOD = {5, 10, 15};

	UPROPERTY(EditAnywhere, config, Category = Anim);
	TArray<FName> CommonCrowdContextualAnimNames;

private:

	UFUNCTION()
	static TArray<FString> GetContextualAnimationOptions()
	{
		TArray<FString> contextualAnimationNames;

		for (const FName& animationName : UCClusterAnimationSettings::Get()->CommonCrowdContextualAnimNames)
		{
			if (animationName != NAME_None)
			{
				contextualAnimationNames.Add(animationName.ToString());
			}
		}

		return contextualAnimationNames;
	}
};
