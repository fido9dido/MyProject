// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"

#include "ActorFragment.generated.h"

USTRUCT()
struct MASSMODULE_API FActorFragment : public FObjectWrapperFragment
{
	GENERATED_BODY()

	enum class ECActorAccess
	{
		OnlyWhenAlive, // Only return an actor pointer if the actor is alive. This is the default.
		IncludePendingKill, // Return an actor pointer even if the actor is marked for destruction.
		IncludeUnreachable // Return an actor pointer even if the actor is unreachable. This implies it's being destroyed.
	};

	/**
	 * Set the actor associated to a mass agent, will also keep the map back in MassActorSubsystem up to date.
	 * @param MassAgent to associated with the actor
	 * @param InActor the actor associated with the mass agent
	 * @param bInIsOwnedByMass tell whether the actors was spawned by mass(MassVisualization) or externally(ReplicatedActors)
	 */
	void SetAndUpdateHandleMap(const FMassEntityHandle massAgent, AActor* inActor, const bool bInIsOwnedByMass);

	/** Resets the actor pointed by this fragment, will also keep the map back in MassActorSubsystem up to date */
	void ResetAndUpdateHandleMap();


	/**
	 * Set the actor associated to a mass agent, will NOT keep map back in MassActorSubsystem up to date.
	 * The user needs to do the map update by hand.
	 * Useful in multithreaded environment, to queue the update of the map inside a deferred command
	 * @param MassAgent to associated with the actor
	 * @param InActor the actor associated with the mass agent
	 * @param bInIsOwnedByMass tell whether the actors was spawned by mass(MassVisualization) or externally(ReplicatedActors)
	 */
	void SetNoHandleMapUpdate(const FMassEntityHandle massAgent, AActor* inActor, const bool bInIsOwnedByMass);

	/** Resets the actor pointed by this fragment, will NOT keep map back in MassActorSubsystem up to date.
	 * The user needs to do the map update by hand.
	 * Useful in multithreaded environment, to queue the update of the map inside a deferred command
	 */
	void ResetNoHandleMapUpdate();


	/** @return none const pointer to the actor	*/
	FORCEINLINE AActor* GetMutable() { return Actor.Get(); }
	AActor* GetMutable(ECActorAccess access);

	/** @return none const pointer to the actor	only if owned by mass */
	FORCEINLINE AActor* GetOwnedByMassMutable() { return bIsOwnedByMass ? Actor.Get() : nullptr; }

	/** @return none const pointer to the actor	only if owned by mass */
	FORCEINLINE const AActor* Get() const { return Actor.Get(); }
	const AActor* Get(ECActorAccess access) const;

	/** @return if the actor is owned by mass */
	FORCEINLINE bool IsOwnedByMass() const { return bIsOwnedByMass; }

	/** @return if the actor is a valid pointer */
	FORCEINLINE bool IsValid() const { return Actor.IsValid(); }

private:

	//visible for debuggingpurposes. 
	UPROPERTY(VisibleAnywhere, Category = "CrowdCluster", Transient)
	TWeakObjectPtr<AActor> Actor;

	/** Ownership of the actor */
	bool bIsOwnedByMass = false;
};
