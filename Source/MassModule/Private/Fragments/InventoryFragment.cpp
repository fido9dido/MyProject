#include "Fragments/InventoryFragment.h"
#include "SmartObjectSubsystem.h"

int32 FInventoryFragment::DepositResource(ECResourceType resourceType, int32 amount)
{
	if (AvailableSpace < 1) { return amount; }

	int32 amountToDeposit = amount;
	if (bool bInventoryOverflow = AvailableSpace < amountToDeposit)
	{
		amountToDeposit = AvailableSpace;
	}

	Inventory[resourceType] += amountToDeposit;
	AvailableSpace -= amountToDeposit;

	return amount - amountToDeposit;

}

int32 FInventoryFragment::WithdrawResource(ECResourceType resourceType, int32 amount)
{
	int32& resourceAmount = Inventory[resourceType];
	if (resourceAmount < 1) { return 0; }

	int32 amountToWithdraw = amount;
	if (resourceAmount < amountToWithdraw)
	{
		amountToWithdraw = resourceAmount;
	}

	resourceAmount -= amountToWithdraw;
	AvailableSpace += amountToWithdraw;
	return amountToWithdraw;
}
