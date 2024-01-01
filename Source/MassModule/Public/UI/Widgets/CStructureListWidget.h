// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DataAssets/CResourceDataAsset.h"
#include "CStructureListWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCStructureListWidget : public UCommonUserWidget
{
	GENERATED_BODY()	

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> StructureList;
public:

	void SetListItems(TArray<TObjectPtr<class UCStructureDataAsset>>& items);
	bool AddStructure(TObjectPtr<UObject> item);
protected:				  
	//~ Begin UUserWidget
	virtual void NativeConstruct() override;
	//~End UUserWidget
};
