// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CStructureRepresentationActorManagement.h"
#include "Actors/ICustomRepresentationActor.h"
#include "Actors/CSmartObjectActor.h"
#include "MassEntityManager.h"
#include <Subsystems/CPlacementSubsystem.h>
#include "MassCrowdSpawnerSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Fragments/SmartObjectFragment.h"

AActor* UCStructureRepresentationActorManagement::GetOrSpawnActor(UMassRepresentationSubsystem& representationSubsystem, FMassEntityManager& entityManager, const FMassEntityHandle massAgent, const FTransform& transform, const int16 templateActorIndex, FMassActorSpawnRequestHandle& spawnRequestHandle, const float priority) const
{
	AActor* spawnedActor = Super::GetOrSpawnActor(representationSubsystem, entityManager, massAgent, transform, templateActorIndex, spawnRequestHandle, priority);
	
	if (!spawnedActor)
	{
		return nullptr;
	}

	if (ICustomRepresentationActor* customRepresentationInterface = Cast<ICustomRepresentationActor>(spawnedActor))
	{
		customRepresentationInterface->OnGetOrSpawn(entityManager, massAgent);
	}

	if (ACSmartObjectActor* smartObjectActor = Cast<ACSmartObjectActor>(spawnedActor))
	{
		if (FSmartObjectFragment* smartObjectFragment = entityManager.GetFragmentDataPtr<FSmartObjectFragment>(massAgent))
		{
			smartObjectFragment->RegisteredHandle = smartObjectActor->GetRegisteredHandle();
		}
	}

	return spawnedActor;
}

void UCStructureRepresentationActorManagement::TeleportActor(const FTransform& transform, AActor& actor, FMassCommandBuffer& commandBuffer) const
{
	FTransform rootTransform = transform;

	if (const UCapsuleComponent* capsuleComp = actor.FindComponentByClass<UCapsuleComponent>())
	{
		const FVector halfHeight(0.0f, 0.0f, capsuleComp->GetScaledCapsuleHalfHeight());
		rootTransform.AddToTranslation(halfHeight);

		const FVector rootLocation = rootTransform.GetLocation();
		const FVector sweepOffset(0.0f, 0.0f, 20.0f);
		const FVector start = rootLocation + sweepOffset;
		const FVector end = rootLocation - sweepOffset;
		FCollisionQueryParams params;
		params.AddIgnoredActor(&actor);
		FHitResult OutHit;
		if (actor.GetWorld()->SweepSingleByChannel(OutHit, start, end, transform.GetRotation(), capsuleComp->GetCollisionObjectType(), capsuleComp->GetCollisionShape(), params))
		{
			rootTransform.SetLocation(OutHit.Location);
		}
	}
	UMassRepresentationActorManagement::TeleportActor(rootTransform, actor, commandBuffer);
}
