Unreal Engine 5.4.0 - MassECS Sample 

This is MassECS Sample, It's not intended to be a complete game or to behave as one!

Currently it has

*	a spawner to spawn resource
*	whitebox structure works as a query
*	basic UI to Spawn Structures and workers on demand
*	state Tree to claimed and deposit resources
*	navigation Mesh for path finding
*   UISetting in Project Settings to Add More structures
*   Press P to enable or disable placement

Open Test Map(or use the default map)

1- Create an Empty Map with NavMesh 
2- Add a RockSpawner in the Map to spawn resources
Play the editor 

1- Enable the Preview by pressing P (Player movement will be disabled until you disable it) 
2- Select Structure and click anywhere on the map to spawn the structure near the resources within 5000 radius(value can be modified)
3- Keep spawning then Press P to Exit Placement Mode
3- Click on the spawned structure to open the UI(Player movement will be disabled, until you close the widget)
4- Click on the + button to add workers, The - button is not implemented

The spawned Charachters will go to gather resources and deposit it in the structure

-----------------------------------------------------------------------------------------------

You need to add these functions to the engine

USmartObjectSubsystem.cpp
 
   	bool USmartObjectSubsystem::UnMarkOccupiedSlot(const FSmartObjectClaimHandle& ClaimHandle)
  	{
	
 	 	FSmartObjectRuntime* SmartObjectRuntime = nullptr;
 	 	FSmartObjectRuntimeSlot* Slot = nullptr;
 	 	FSmartObjectSlotHandle SlotHandle = ClaimHandle.SlotHandle;
	
	 	if (!ClaimHandle.IsValid())
	 	{
		 	return false;
	 	}
	
	 	if (!GetValidatedMutableRuntimeAndSlot(SlotHandle, SmartObjectRuntime, Slot, ANSI_TO_TCHAR(__FUNCTION__)))
	 	{
		 	return false;
	 	}

	 	if (Slot->GetState() == ESmartObjectSlotState::Claimed)
	 	{
	 		return true;
	 	}

	 	if (ensureMsgf(Slot->GetState() == ESmartObjectSlotState::Occupied ||
		 	Slot->GetState() == ESmartObjectSlotState::Claimed
		 	, TEXT("Should have been occupied first: %s"), *LexToString(ClaimHandle)))
	 	{
		 	Slot->State = ESmartObjectSlotState::Claimed;
		 	return true;
	 	}
	 	return false;

  	}
 

MassEntityManager.h

	template<typename T>
	const T* GetConstSharedFragment()
	{
		static_assert(TIsDerivedFrom<T, FMassSharedFragment>::IsDerived, "Given struct doesn't represent a valid Shared fragment type. Make sure to inherit from FMassSharedFragment or one of its child-types.");

		for (FConstSharedStruct& Element : ConstSharedFragments)
		{
			if (const T* Data = Element.GetPtr<const T>())
			{
				return	Data;
			}
		}
		return nullptr;
	}

	template<typename T>
	const T* GetSharedFragment()
	{
		static_assert(TIsDerivedFrom<T, FMassSharedFragment>::IsDerived, "Given struct doesn't represent a valid Shared fragment type. Make sure to inherit from FMassSharedFragment or one of its child-types.");

		for (FConstSharedStruct& Element : SharedFragments)
		{
			if (const T* Data = Element.GetPtr<const T>())
			{
				return	Data;
			}
		}
		return nullptr;
	}
