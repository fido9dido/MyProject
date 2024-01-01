// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"   
#include "MassEntityTemplate.h"
#include "Misc/MTAccessDetector.h"
#include "UObject/ObjectKey.h"
#include "Subsystems/WorldSubsystem.h"
#include "Fragments/ActorFragment.h"
#include "ActorSubsystem.generated.h"

//This class is not being used for now, probably never

struct MASSMODULE_API FActorManager : public TSharedFromThis<FActorManager>
{
protected:

	TMap<TObjectKey<const AActor>, FMassEntityHandle> ActorHandleMap;
	UE_MT_DECLARE_RW_ACCESS_DETECTOR(ActorHandleMapDetector);

	TSharedPtr<FMassEntityManager> EntityManager;

	TWeakObjectPtr<UObject> Owner;
public:
	explicit FActorManager(const TSharedPtr<FMassEntityManager>& entityManager, UObject* inOwner = nullptr);
	FMassEntityHandle GetEntityHandleFromActor(const TObjectKey<const AActor> actor);
	void SetHandleForActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle);
	void RemoveHandleForActor(const TObjectKey<const AActor> actor);
	AActor* GetActorFromHandle(const FMassEntityHandle handle, FActorFragment::ECActorAccess access = FActorFragment::ECActorAccess::OnlyWhenAlive) const;
	void DisconnectActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle);

};

UCLASS()
class MASSMODULE_API UActorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	TSharedPtr<FActorManager> ActorManager;
public:
	inline FMassEntityHandle GetEntityHandleFromActor(const TObjectKey<const AActor> actor);
	inline void SetHandleForActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle);
	inline void RemoveHandleForActor(const TObjectKey<const AActor> actor);
	inline AActor* GetActorFromHandle(const FMassEntityHandle handle, FActorFragment::ECActorAccess access = FActorFragment::ECActorAccess::OnlyWhenAlive) const;
	inline void DisconnectActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle);

protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

};

template<>
struct TMassExternalSubsystemTraits<UActorSubsystem> final
{
	enum
	{
		GameThreadOnly = false
	};
};

FMassEntityHandle UActorSubsystem::GetEntityHandleFromActor(const TObjectKey<const AActor> actor)
{
	return ActorManager->GetEntityHandleFromActor(actor);
}

void UActorSubsystem::SetHandleForActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle)
{
	ActorManager->SetHandleForActor(actor, handle);
}

void UActorSubsystem::RemoveHandleForActor(const TObjectKey<const AActor> actor)
{
	ActorManager->RemoveHandleForActor(actor);
}

AActor* UActorSubsystem::GetActorFromHandle(const FMassEntityHandle handle, FActorFragment::ECActorAccess access) const
{
	return ActorManager->GetActorFromHandle(handle, access);
}

void UActorSubsystem::DisconnectActor(const TObjectKey<const AActor> actor, const FMassEntityHandle handle)
{
	return ActorManager->DisconnectActor(actor, handle);
}
