// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CStructureToolTipWidget.generated.h"

/**
 * 		
 */
UCLASS()
class UCStructureToolTipWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	class UTileView* CostTileView;

public:
	void SyncData(class UCStructureDataAsset* structureData);
	void SetName(FText name);
	void SetDescription(FText description);
	
};
