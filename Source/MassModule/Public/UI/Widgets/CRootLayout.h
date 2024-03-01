// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"	  
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameplayTagContainer.h"
#include "Utilities/Util.h"
#include "CRootLayout.generated.h"

/**
 * The state of an async load operation for the UI.
 */
enum class ECAsyncWidgetLayerState : uint8
{
	Initialize,
	Canceled,
	Active
};

/**
 * 	Simplified Version of Lyra
 */
UCLASS()
class UCRootLayout : public UCommonUserWidget
{
	GENERATED_BODY()

private:			   
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonActivatableWidgetStack> GameLayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonActivatableWidgetStack> ModalLayer;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonActivatableWidgetStack> MenuLayer;

	// The registered layers for the primary layout.
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
	
	// Lets us keep track of all suspended input tokens so that multiple async UIs can be loading and we correctly suspend
	// for the duration of all of them.
	TArray<FName> SuspendInputTokens;

	bool bIsDormant = false;

public:
	class UCHUDWidget* GetHud();
	// Get the layer widget for the given layer tag.
	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag layerName);

	template <typename ActivatableWidgetT>
	ActivatableWidgetT* GetWidgetOfTypeInLayer(FGameplayTag layer)
	{
		if (!Layers.Contains(layer)) { return nullptr; }
		TObjectPtr<UCommonActivatableWidgetContainerBase>& requestedLayer = Layers[layer];
		const TArray<UCommonActivatableWidget*>& widgetList = requestedLayer->GetWidgetList();

		for (UCommonActivatableWidget* widget : widgetList)
		{
			if (ActivatableWidgetT* requestedWidget = Cast<ActivatableWidgetT>(widget))
			{
				return requestedWidget;
			}
		}

		return nullptr;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> AddWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass)
	{
		return AddWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, bSuspendInputUntilComplete, ActivatableWidgetClass, [](ECAsyncWidgetLayerState, ActivatableWidgetT*) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> AddWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass, TFunction<void(ECAsyncWidgetLayerState, ActivatableWidgetT*)> StateFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
		const FName SuspendInputToken = bSuspendInputUntilComplete ? FUtil::SetInputForPlayer(GetOwningPlayer(), false, NAME_PushingWidgetToLayer) : NAME_None;

		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(ActivatableWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,
			[this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken]()
			{
				FUtil::SetInputForPlayer(GetOwningPlayer(), true, SuspendInputToken);

				ActivatableWidgetT* Widget = AddWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT& WidgetToInit) {
					StateFunc(ECAsyncWidgetLayerState::Initialize, &WidgetToInit);
					});

				StateFunc(ECAsyncWidgetLayerState::Active, Widget);
			})
		);

		// Setup a cancel delegate so that we can resume input if this handler is canceled.
		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
			[this, StateFunc, SuspendInputToken]()
			{
				FUtil::SetInputForPlayer(GetOwningPlayer(), true, SuspendInputToken);
				StateFunc(ECAsyncWidgetLayerState::Canceled, nullptr);
			})
		);

		return StreamingHandle;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* AddWidgetToLayerStack(FGameplayTag layerName, UClass* activatableWidgetClass)
	{
		return AddWidgetToLayerStack<ActivatableWidgetT>(layerName, activatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* AddWidgetToLayerStack(FGameplayTag layerName, UClass* activatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> initInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase* layer = GetLayerWidget(layerName))
		{
			return layer->AddWidget<ActivatableWidgetT>(activatableWidgetClass, initInstanceFunc);
		}

		return nullptr;
	}

	// Find the widget if it exists on any of the layers and remove it from the layer.
	void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* activatableWidget);

protected:
	virtual void NativeOnInitialized() override;
	/** Register a layer that widgets can be pushed onto. */
	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer( FGameplayTag layerTag, UCommonActivatableWidgetContainerBase* layerWidget);
	
	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* widget, bool bIsTransitioning);

};


