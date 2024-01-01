// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/CClusterSettings.h"
#include "MassEntityConfigAsset.h"

void UCClusterSettings::PostInitProperties() 
{
	Super::PostInitProperties();

}

#if WITH_EDITOR
void UCClusterSettings::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

}
#endif // WITH_EDITOR
