// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/Util.h"
#include "NavigationData.h"
#include "NavigationSystem.h" 

#include "MassSpawnerSubsystem.h"
#include "MassEntityTemplate.h"
#include "MassEntityConfigAsset.h"
#include "MassSpawnerTypes.h"
#include "MassSpawnLocationProcessor.h"
#include "CommonInputTypeEnum.h"
#include "CommonInputSubsystem.h"

int32 FUtil::InputSuspensionCount = 0;

void FUtil::DrawDebugPath(UWorld* world, TArray<FNavPathPoint> pathPoints, FColor pathColor, UCanvas* canvas, bool bPersistent, float lifeTime, const uint32 nextPathPointIndex)
{
#if ENABLE_DRAW_DEBUG

	static const FColor grey(100, 100, 100);
	const int32 numPathVerts = pathPoints.Num();

	for (int32 i = 0; i < numPathVerts - 1; ++i)
	{
		// draw box at vert
		FVector const lineStart = pathPoints[i].Location + NavigationDebugDrawing::PathOffset;
		DrawDebugSolidBox(world, lineStart, NavigationDebugDrawing::PathNodeBoxExtent, i < int32(nextPathPointIndex) ? grey : pathColor, bPersistent, lifeTime);

		// draw line to next loc
		FVector const lineEnd = pathPoints[i + 1].Location + NavigationDebugDrawing::PathOffset;
		DrawDebugLine(world, lineStart, lineEnd, i < int32(nextPathPointIndex) - 1 ? grey : pathColor, bPersistent, lifeTime, 0, NavigationDebugDrawing::PathLineThickness);
	}

	// draw last vert
	if (numPathVerts > 0)
	{
		DrawDebugBox(world, pathPoints[numPathVerts - 1].Location + NavigationDebugDrawing::PathOffset, FVector(15.), pathColor, bPersistent, lifeTime);
	}

#endif
}

TArray<FMassEntityHandle> FUtil::SpawnEntity(UMassEntityConfigAsset* entityConfig, UWorld& world, int32 numToSpawn, const TArray<FTransform>& transformList)
{
	if (!entityConfig) { return TArray<FMassEntityHandle>(); }
	UMassSpawnerSubsystem* spawnerSubsystem = UWorld::GetSubsystem<UMassSpawnerSubsystem>(&world);

	const FMassEntityTemplate& entityTemplate = entityConfig->GetOrCreateEntityTemplate(world);

	FMassTransformsSpawnData SpawnLocationData;
	SpawnLocationData.Transforms = transformList;

	TArray<FMassEntityHandle> outEntities;
	spawnerSubsystem->SpawnEntities(entityTemplate.GetTemplateID(), numToSpawn, FConstStructView::Make(SpawnLocationData), UMassSpawnLocationProcessor::StaticClass(), outEntities);

	return outEntities;
}

FName FUtil::SetInputForPlayer(APlayerController* PlayerController, bool bEnable, FName Reason)
{
	return SetInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, bEnable, Reason);
}

FName FUtil::SetInputForPlayer(ULocalPlayer* LocalPlayer, bool bEnable, FName Reason)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		InputSuspensionCount++;
		FName SuspendToken = Reason;
		SuspendToken.SetNumber(InputSuspensionCount);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);

		return SuspendToken;
	}

	return NAME_None;
}
