// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CStructureToolTipWidget.h"
#include "Components/TileView.h"
#include "CommonTextBlock.h"
#include "DataAssets/CResourceDataAsset.h"
#include "DataAssets/CStructureDataAsset.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CStructureToolTipWidget)

void UCStructureToolTipWidget::SyncData(const UCStructureDataAsset* structureData)
{
	SetName(structureData->Name);
	SetDescription(structureData->Description);
	
	if (CostTileView)
	{
		CostTileView->SetListItems(structureData->ResourceCostList);
	}
}

void UCStructureToolTipWidget::SetName(FText name)
{
	if (Name)
	{
		Name->SetText(name);
	}
}

void UCStructureToolTipWidget::SetDescription(FText description)
{
	if (Description)
	{
		Description->SetText(description);
	}
}
