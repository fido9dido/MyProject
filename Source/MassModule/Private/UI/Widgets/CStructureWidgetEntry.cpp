#include "UI/Widgets/CStructureWidgetEntry.h"
#include "UI/Widgets/CStructureToolTipWidget.h"
#include "GameSettings/CTopDownPlayerController.h"
#include "DataAssets/CStructureDataAsset.h"
#include "Subsystems/CPlacementSubsystem.h"
#include "Utilities/MassUtil.h"

#include <Components/Image.h>
#include <CommonTextBlock.h>

void UCStructureWidgetEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TSubclassOf<UCStructureToolTipWidget> structureToolTipWidget = EntryTooltip.LoadSynchronous();
	UCStructureToolTipWidget* entryTooltip = CreateWidget<UCStructureToolTipWidget>(this, structureToolTipWidget);
	SetToolTip(entryTooltip);
}

void UCStructureWidgetEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!ListItemObject) { return; }
	
	UCStructureDataAsset* structureData = Cast<UCStructureDataAsset>(ListItemObject);
	if (!structureData) { return; }
	
	StructureData = structureData;

	if (Image)
	{
		Image->SetBrushResourceObject(structureData->Image);
	}

	if (!GetToolTip()) { return; }
	
	UCStructureToolTipWidget* sructureToolTipWidget = Cast<UCStructureToolTipWidget>(GetToolTip());
	sructureToolTipWidget->SyncData(structureData);
	
}

FReply UCStructureWidgetEntry::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	ACTopDownPlayerController* playerController = Cast<ACTopDownPlayerController>(GetOwningPlayer());
	playerController->SetInputMode(FInputModeGameOnly());
	playerController->SetInteractingStatus(true);
	
	UCStructureDataAsset* structureData = StructureData.LoadSynchronous();
	FMassUtil::SendStructureDataToPreviewEntity(structureData, GetWorld());

	return FReply::Handled();
}