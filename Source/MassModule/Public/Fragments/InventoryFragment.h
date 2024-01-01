// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassCommonFragments.h"
#include "Containers/Map.h"
#include "GlobalTypes.h"

#include "InventoryFragment.generated.h"

// Simple Inventory
USTRUCT()
struct MASSMODULE_API FInventoryFragment : public FMassFragment
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 MaxCapacity = 10;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 AvailableSpace = 10;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TMap<ECResourceType, int32> Inventory ;

public:

	bool IsEmpty() { return MaxCapacity == AvailableSpace; }
	bool IsValidType(ECResourceType& type) { return Inventory.Contains(type); }

	/*
	 * Deposit Any resource amount within the inventory capacity and return extra amount incase of overflow 
	 */
	int32 DepositResource(ECResourceType resourceType, int32 amount);
	/*
	 * Withdraw and return the actual amount removed from the inventory
	 */
	int32 WithdrawResource(ECResourceType resourceType, int32 amount);

	int32& operator[](ECResourceType index) { return Inventory[index]; }
	TMap<ECResourceType, int32>::TRangedForIterator begin() { return Inventory.begin(); }
	TMap<ECResourceType, int32>::TRangedForIterator end() { return Inventory.end(); }
	TMap<ECResourceType, int32>& Get() { return Inventory; }
};
