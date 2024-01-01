// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct GLOBALSMODULE_API FUtil
{
	static void DrawDebugPath(class UWorld* world, TArray<FNavPathPoint> pathPoints, FColor pathColor, UCanvas* canvas, bool bPersistent, float lifeTime, const uint32 nextPathPointIndex = 0);
	static TArray<struct FMassEntityHandle> SpawnEntity(class UMassEntityConfigAsset* entityConfig, class UWorld& world, int32 numToSpawn, const TArray<FTransform>& transformList);

	static FName SetInputForPlayer(APlayerController* PlayerController, bool bEnable, FName Reason);
	static FName SetInputForPlayer(ULocalPlayer* LocalPlayer, bool bEnable, FName Reason);
	
private:
	static int32 InputSuspensionCount;

};