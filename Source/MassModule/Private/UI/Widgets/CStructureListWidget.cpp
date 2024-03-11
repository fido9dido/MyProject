// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CStructureListWidget.h"
#include "DataAssets/CStructureDataAsset.h"
#include <Components/ListView.h>
#include <GameSettings/CUISettings.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(CStructureListWidget)

UCStructureListWidget::UCStructureListWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//DeactivateWidget();
}

void UCStructureListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PopulateStructureList();
}

void UCStructureListWidget::PopulateStructureList()
{
	UCUISettings* uiSettings = GetMutableDefault<UCUISettings>();

	TArray<TObjectPtr<UCStructureDataAsset>> tempDataList;
	for (TSoftObjectPtr<UCStructureDataAsset>& structureSoftPtr : uiSettings->StructuretDataList)
	{
		UCStructureDataAsset* structureData = structureSoftPtr.LoadSynchronous();
		if (structureData)
		{
			tempDataList.Emplace(structureData);
		}
	}

	StructureList->SetListItems(tempDataList);

}

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
