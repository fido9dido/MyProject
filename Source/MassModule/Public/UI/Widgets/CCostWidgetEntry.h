// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CCostWidgetEntry.generated.h"

/**
 * 		
 */
UCLASS()
class UCCostWidgetEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Name; 
	
	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Count;
protected:

	//~ Begin IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry


};
