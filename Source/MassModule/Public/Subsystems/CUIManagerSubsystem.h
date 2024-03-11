// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include <GameplayTagContainer.h>

#include "CUIManagerSubsystem.generated.h"

/**
 *	
 */
UCLASS(config = Game)
class MASSMODULE_API UCUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TObjectPtr<class UCRootLayout> RootLayout = nullptr;

	TMap<FGameplayTag, TObjectPtr<class UCommonActivatableWidget>> PlaceholderMap;
	TArray<TSoftObjectPtr<class UCHUDSlotDefinition>> HUDSlotDefinition;

public:
	UCUIManagerSubsystem() = default;
	
	void OnBeginPlay(UWorld& world);

	virtual void Initialize(class FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	const class UCRootLayout* GetRootLayout() const;
	class UCRootLayout* GetRootLayout();
	void PushStreamedContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UCommonActivatableWidget> WidgetClass);
	void PopContentFromLayer(class UCommonActivatableWidget* activatableWidget);
	void AddPlaceholderWidget(FGameplayTag slotTag, TObjectPtr<class UCommonActivatableWidget>& entryWidget);
	void RemovePlaceholderWidget(FGameplayTag& slotTag);
	
	TObjectPtr<UCommonActivatableWidget> FindPlaceholderWidget(FGameplayTag& slotTag);

protected:
	void CreateLayoutWidget(class ULocalPlayer* LocalPlayer, TSubclassOf<class UCommonUserWidget> layoutWidgetClass);
	void AddLayoutToViewport(class ULocalPlayer* LocalPlayer, class UCRootLayout* Layout);
	void OnRootLayoutAddedToViewport(class ULocalPlayer* LocalPlayer, class UCRootLayout* Layout);
	void SetRootLayout(class UCRootLayout* layout);

};
											