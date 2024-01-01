// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/ActorSubsystem.h"
#include "MassCommonTypes.h"
#include "Engine/World.h"
#include "MassEntityManager.h"
#include "MassEntitySubsystem.h"
#include "MassSimulationSubsystem.h"
#include "VisualLogger/VisualLogger.h"

//----------------------------------------------------------------------//
//  UMassActorSubsystem 
//----------------------------------------------------------------------//
void UActorSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	// making sure UMassSimulationSubsystem gets created before the MassActorSubsystem
	collection.InitializeDependency<UMassSimulationSubsystem>();

	if (UMassEntitySubsystem* entitySubsystem = UWorld::GetSubsystem<UMassEntitySubsystem>(GetWorld()))
	{
		ActorManager = MakeShareable(new FActorManager(entitySubsystem->GetMutableEntityManager().AsShared()));
	}
}

void UActorSubsystem::Deinitialize()
{
	ActorManager.Reset();
}

//----------------------------------------------------------------------//
//  FMassActorManager
//----------------------------------------------------------------------//
FActorManager::FActorManager(const TSharedPtr<FMassEntityManager>& inEntityManager, UObject* inOwner)
	: EntityManager(inEntityManager)
	, Owner(inOwner)
{
}

FMassEntityHandle FActorManager::GetEntityHandleFromActor(const TObjectKey<const AActor> actor)
{
	UE_MT_SCOPED_READ_ACCESS(ActorHandleMapDetector);
	FMassEntityHandle* entity = ActorHandleMap.Find(actor);
	if (!entity)
	{
		return FMassEntityManager::InvalidEntity;
	}

	check(TObjectKey<const AActor>(GetActorFromHandle(*entity, FActorFragment::ECActorAccess::IncludeUnreachable)) == actor);
	return *entity;
}

AActor* FActorManager::GetActorFromHandle(const FMassEntityHandle handle, FActorFragment::ECActorAccess access) const
{
	check(EntityManager);
	FActorFragment* data = EntityManager->GetFragmentDataPtr<FActorFragment>(handle);
	return data != nullptr ? data->GetMutable(access) : nullptr;
}

void FActorManager::SetHandleForActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle)
{
	UE_MT_SCOPED_WRITE_ACCESS(ActorHandleMapDetector);
	ActorHandleMap.Add(actor, handle);
}

void FActorManager::RemoveHandleForActor(const TObjectKey<const AActor> actor)
{
	UE_MT_SCOPED_WRITE_ACCESS(ActorHandleMapDetector);
	ActorHandleMap.Remove(actor);
}

void FActorManager::DisconnectActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle)
{
	if (handle.IsValid() == false)
	{
		return;
	}

	FMassEntityHandle foundEntity;
	{
		UE_MT_SCOPED_WRITE_ACCESS(ActorHandleMapDetector);
		// We're assuming the Handle does match Actor, so we're RemoveAndCopyValue. If if doesn't we'll add it back.
		// The expectation is that this won't happen on a regular basis..
		if (ActorHandleMap.RemoveAndCopyValue(actor, foundEntity) == false)
		{
			// the entity doesn't match the actor
			return;
		}
	}

	if (foundEntity == handle)
	{
		check(EntityManager);
		if (FActorFragment* data = EntityManager->GetFragmentDataPtr<FActorFragment>(handle))
		{
			data->ResetAndUpdateHandleMap();
		}
	}
	else
	{
		// unexpected mismatch.
		SetHandleForActor(actor, handle);
	}
}
