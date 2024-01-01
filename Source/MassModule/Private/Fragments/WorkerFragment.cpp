#include "Fragments/WorkerFragment.h"
#include "SmartObjectSubsystem.h"

void FWorkerFragment::OnJobDone(USmartObjectSubsystem& smartObjectSubsystem)
{
	smartObjectSubsystem.MarkSlotAsFree(JobClaimHandle);
	JobClaimHandle.Invalidate();
}
