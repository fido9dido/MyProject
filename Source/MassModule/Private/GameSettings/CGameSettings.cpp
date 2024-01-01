// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/CGameSettings.h"
#include "MassEntityConfigAsset.h"

UCGameSettings* UCGameSettings::GetMutable()
{
	return GetMutableDefault<UCGameSettings>();
}

const UCGameSettings* UCGameSettings::Get()
{
	return GetDefault<UCGameSettings>();
}

UMassEntityConfigAsset* UCGameSettings::GetPreviewConfig() const
{
	return PreviewConfigAsset.LoadSynchronous();
}	

TSubclassOf<AActor> UCGameSettings::GetPreviewActorClass()
{
	return PreviewActorClass.LoadSynchronous();
}

