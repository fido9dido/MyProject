#include "UI/Widgets/CStructureWidgetEntry.h"
#include "UI/Widgets/CStructureToolTipWidget.h"
#include "Characters/CTopDownPlayerController.h"
#include "DataAssets/CStructureDataAsset.h"
#include "Utilities/MassUtil.h"
#include "Actors/CPreviewActor.h"

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

	UCStructureDataAsset* structureData = StructureData.LoadSynchronous();
	
	if (structureData)
	{
		FMassUtil::SendStructureDataToPreviewEntity(structureData, GetWorld());
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}