// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseActivatableWidget.h"
#include "CHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCHUDWidget : public UCBaseActivatableWidget
{
	GENERATED_BODY()	

public:
	UPROPERTY(meta = (BindWidget))
	TSoftObjectPtr<class UCStructureListWidget> StructureListWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> StructureMenuClass;

public:
	UCHUDWidget(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UUserWidget
	void NativeOnInitialized() override;
	//~ End UUserWidget
protected:
	void PopulateStructureList();
	void HandleEscapeAction();
};
