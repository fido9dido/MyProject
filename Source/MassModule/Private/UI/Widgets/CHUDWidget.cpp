// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CHUDWidget.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "Input/CommonUIInputTypes.h"
#include <Subsystems/CUIManagerSubsystem.h>
#include <GameSettings/CUISettings.h>
#include "UI/Widgets/CStructureListWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CHUDWidget)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_STRUCTURE_MENU, "UI.Layer.Structure.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

UCHUDWidget::UCHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
	PopulateStructureList();
}

void UCHUDWidget::PopulateStructureList()
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

	if (StructureListWidget)
	{
		StructureListWidget->SetListItems(tempDataList);
	}
}

void UCHUDWidget::HandleEscapeAction()
{
	UCUIManagerSubsystem* uiManagerSubsystem = GetGameInstance()->GetSubsystem<UCUIManagerSubsystem>();
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		uiManagerSubsystem->PushStreamedContentToLayer(TAG_UI_LAYER_STRUCTURE_MENU, EscapeMenuClass);
	}
}
