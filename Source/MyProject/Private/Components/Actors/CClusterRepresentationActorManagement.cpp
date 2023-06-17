// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CClusterRepresentationActorManagement.h"
#include "Actors/IClusterActor.h"
#include "MassEntityManager.h"

#include "MassCrowdSpawnerSubsystem.h"
#include "Components/CapsuleComponent.h"

EMassActorSpawnRequestAction UCClusterRepresentationActorManagement::OnPostActorSpawn(const FMassActorSpawnRequestHandle& spawnRequestHandle, FConstStructView spawnRequest, FMassEntityManager* entitySubsystem) const
{
	check(entitySubsystem);

	const EMassActorSpawnRequestAction result = Super::OnPostActorSpawn(spawnRequestHandle, spawnRequest, entitySubsystem);
	
	const FMassActorSpawnRequest& massActorSpawnRequest = spawnRequest.Get<FMassActorSpawnRequest>();
	checkf(massActorSpawnRequest.SpawnedActor, TEXT("Expecting valid spawned actor"));

	if (IClusterActor* bulkActorInterface = Cast<IClusterActor>(massActorSpawnRequest.SpawnedActor))
	{
		bulkActorInterface->OnGetOrSpawn(entitySubsystem, massActorSpawnRequest.MassAgent);
	}

	return result;
}

void UCClusterRepresentationActorManagement::TeleportActor(const FTransform& transform, AActor& actor, FMassCommandBuffer& commandBuffer) const
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
