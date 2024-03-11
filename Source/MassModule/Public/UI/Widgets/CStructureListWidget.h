// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "DataAssets/CResourceDataAsset.h"
#include "CStructureListWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCStructureListWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()	

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> StructureList;

public:
	UCStructureListWidget(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UUserWidget 
	virtual void NativeConstruct() override;
	//~ End UUserWidget

	void PopulateStructureList();

	void SetListItems(const TArray<TObjectPtr<class UCStructureDataAsset>>& items);
	bool AddStructure(const TObjectPtr<UObject> item);
};
