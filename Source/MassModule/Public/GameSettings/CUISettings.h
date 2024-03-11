// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "DataAssets/CStructureDataAsset.h"

#include "CUISettings.generated.h"

UCLASS()
class UCWidgetDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonActivatableWidget> Widget;
	UPROPERTY(EditAnywhere)
	FGameplayTag LayerTag;
};

UCLASS()
class UCPlaceholderSlotDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonActivatableWidget> Widget;

	UPROPERTY(EditAnywhere)
	FGameplayTag SlotTag;
};

UCLASS(config=Game, defaultconfig, meta=(DisplayName="UI Settings"))
class MASSMODULE_API UCUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category = "DefaultLayout", meta = (AllowedClasses = "CWidgetDefinition"))
	TArray<TSoftObjectPtr<UCWidgetDefinition>> WidgetDefinition;

	UPROPERTY(config, EditAnywhere, Category = "DefaultLayout", meta = (AllowedClasses = "CStructureDataAsset"))
	TArray<TSoftObjectPtr<UCStructureDataAsset>> StructuretDataList;

	UPROPERTY(config, EditAnywhere, Category = "DefaultLayout")
	TSoftClassPtr<UCommonUserWidget> RootLayoutWidgetClass;

	//Widgets to be placed in the HUD's PlaceholderWidget, assumes one slot per placeholder
	UPROPERTY(config, EditAnywhere, Category = "DefaultLayout", meta = (AllowedClasses = "CPlaceholderSlotDefinition"))
	TArray<TSoftObjectPtr<UCPlaceholderSlotDefinition>> PlaceholderSlotDefinition;

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	TSubclassOf<class UCommonUserWidget> GetRootLayoutClass() const;
	TSubclassOf<UUserWidget> FindSlotOfWithTag(FGameplayTag slotTag);
	
};
