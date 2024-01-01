// Fill out your copyright notice in the Description page of Project Settings.

#include "Fragments/ActorFragment.h"
#include "MassCommonTypes.h"
#include <Subsystems/ActorSubsystem.h>

//----------------------------------------------------------------------//
//  FActorFragment 
//----------------------------------------------------------------------//

void FActorFragment::SetAndUpdateHandleMap(const FMassEntityHandle massAgent, AActor* inActor, const bool bInIsOwnedByMass)
{
	SetNoHandleMapUpdate(massAgent, inActor, bInIsOwnedByMass);

	UWorld* world = inActor->GetWorld();
	check(world);
	if (UActorSubsystem* actorSubsystem = UWorld::GetSubsystem<UActorSubsystem>(world))
	{
		actorSubsystem->SetHandleForActor(inActor, massAgent);
	}
}

void FActorFragment::ResetAndUpdateHandleMap()
{
	if (AActor* actorPtr = Cast<AActor>(Actor.Get()))
	{
		UWorld* world = actorPtr->GetWorld();
		check(world);
		if (UActorSubsystem* actorSubsystem = UWorld::GetSubsystem<UActorSubsystem>(world))
		{
			actorSubsystem->RemoveHandleForActor(actorPtr);
		}
	}

	ResetNoHandleMapUpdate();
}

void FActorFragment::SetNoHandleMapUpdate(const FMassEntityHandle massAgent, AActor* inActor, const bool bInIsOwnedByMass)
{
	check(inActor);
	check(!Actor.IsValid());
	check(massAgent.IsValid());
	Actor = inActor;
	bIsOwnedByMass = bInIsOwnedByMass;
}

void FActorFragment::ResetNoHandleMapUpdate()
{
	Actor.Reset();
	bIsOwnedByMass = false;
}

AActor* FActorFragment::GetMutable(ECActorAccess access)
{
	switch (access)
	{
	case ECActorAccess::OnlyWhenAlive:
		return Actor.Get();
	case ECActorAccess::IncludePendingKill:
		return Actor.Get(true);
	case ECActorAccess::IncludeUnreachable:
		return Actor.GetEvenIfUnreachable();
	default:
		checkf(false, TEXT("Invalid ActorAccess value: %i."), static_cast<int32>(access));
		return nullptr;
	}
}

const AActor* FActorFragment::Get(ECActorAccess access) const
{
	return const_cast<FActorFragment*>(this)->GetMutable(access);
}