Unreal Engine 5.4.0 - MassECS Sample 

This is MassECS Sample, to demonstrate how mass works and how to use MassECS with other systems like navigation and smart objects etc
the functionalities are implemented with that in mind, It's not intended to be a game!

Currently it has

*	a spawner to spawn resource
*	whitebox structure works as a query
*	basic UI to Spawn Structures and workers on demand
*	state Tree to claimed and deposit resources
*	navigation Mesh for path finding
*   UISetting in Project Settings to Add More structures
*   Press P to enable or disable placement

create new Map(or use the test map)

Steps to creating an empty Map

1- Create an Empty Map with NavMesh 
2- Add a resource spawner in the map, RockSpawner exists in the editor

Play the editor 

1- Enable the Preview by pressing P (Player movement will be disabled until you disable it) 
2- Select Structure and click anywhere on the map to spawn the structure near the resources within 5000 radius(value can be modified)
3- Keep spawning then Press P to Exit Placement Mode
3- Click on the spawned structure to open the UI(Player movement will be disabled, until you close the widget)
4- Click on the + button to add workers, The - button is not implemented

The spawned Charachters will go to gather resources and deposit it in the structure

-----------------------------------------------------------------------------------------------

You need to add this function to the engine

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
 