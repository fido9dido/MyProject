#include "UI/Widgets/CCostWidgetEntry.h"
#include "DataAssets/CResourceDataAsset.h"
#include "Components/Image.h"
#include "CommonTextBlock.h"

void UCCostWidgetEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UCResourceCost* resourceCost = Cast<UCResourceCost>(ListItemObject);
	if (!resourceCost) { return; }

	UCResourceDataAsset* resourceData = resourceCost->ResourceData;
	if (!resourceData) { return; }

	if (Image)
	{
		Image->SetBrushResourceObject(resourceData->Image);
	}

	if (Name)
	{
		Name->SetText(resourceData->Name);
	} 

	if (Count)
	{
		Count->SetText(FText::AsNumber(resourceCost->Count));
	}
}
