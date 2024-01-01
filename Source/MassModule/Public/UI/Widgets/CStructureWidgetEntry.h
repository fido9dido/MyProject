// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DataAssets/CStructureDataAsset.h"
#include "CStructureWidgetEntry.generated.h"

/**
* 		
*/
UCLASS()
class UCStructureWidgetEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UCStructureToolTipWidget> EntryTooltip;
	TSoftObjectPtr<class UCStructureDataAsset> StructureData;

protected:
	//~ Begin UUserWidget
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//~ End UUserWidget
	
	//~ Begin IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry

	//~ Begin UCommonUserWidget
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End UCommonUserWidget
};
