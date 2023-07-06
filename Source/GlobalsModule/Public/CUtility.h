
#pragma once

#include "NavigationData.h"

struct CUtility
{
	static void DrawDebugPath(UWorld* world, TArray<FNavPathPoint> pathPoints, FColor pathColor, UCanvas* canvas, bool bPersistent, float lifeTime, const uint32 nextPathPointIndex = 0)
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
};