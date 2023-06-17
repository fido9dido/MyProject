// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/CClusterSettings.h"
#include "MassEntityConfigAsset.h"

void UCClusterSettings::PostInitProperties() 
{
	Super::PostInitProperties();

	if (AgentConfigAsset.IsPending())
	{
		AgentConfigAsset.LoadSynchronous();
	}
	AgentConfig = AgentConfigAsset.Get();
}

#if WITH_EDITOR
void UCClusterSettings::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	if (AgentConfigAsset.IsPending())
	{
		AgentConfigAsset.LoadSynchronous();
	}
	AgentConfig = AgentConfigAsset.Get();
}
#endif // WITH_EDITOR
