// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CStructureListWidget.h"
#include "DataAssets/CStructureDataAsset.h"
#include <Components/ListView.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(CStructureListWidget)

void UCStructureListWidget::SetListItems(const TArray<TObjectPtr<UCStructureDataAsset>>& items)
{
	StructureList->SetListItems(items);
}

bool UCStructureListWidget::AddStructure(const TObjectPtr<UObject> item)
{
	if (!item) { return false; };

	if (StructureList)
	{
		StructureList->AddItem(item.Get());
	}

	return true;
}
