// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CRootLayout.h"
#include "UI/Widgets/CHUDWidget.h"
#include "GameSettings/CUISettings.h"
#include <Utilities/Util.h>
#include "Widgets/CommonActivatableWidgetContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CRootLayout)

UCommonActivatableWidgetContainerBase* UCRootLayout::GetLayerWidget(FGameplayTag layerName)
{
	return Layers.FindRef(layerName);
}

void UCRootLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* activatableWidget)
{
	// We're not sure what layer the widget is on so go searching.
	for (const TPair<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>>& LayerKVP : Layers)
	{
		LayerKVP.Value->RemoveWidget(*activatableWidget);
	}
}

void UCRootLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

void UCRootLayout::RegisterLayer(FGameplayTag layerTag, UCommonActivatableWidgetContainerBase* layerWidget)
{
	if (IsDesignTime()||!layerWidget) { return; }
	
	layerWidget->OnTransitioningChanged.AddUObject(this, &UCRootLayout::OnWidgetStackTransitioning);
	layerWidget->SetTransitionDuration(0.0);

	Layers.Add(layerTag, layerWidget);
}

void UCRootLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* widget, bool bIsTransitioning)
{
	if (bIsTransitioning)
	{
		const FName suspendToken = FUtil::SetInputForPlayer(GetOwningLocalPlayer(), false, TEXT("GlobalStackTransion"));
		SuspendInputTokens.Add(suspendToken);
	}
	else
	{
		if (ensure(SuspendInputTokens.Num() > 0))
		{
			const FName suspendToken = SuspendInputTokens.Pop();
			FUtil::SetInputForPlayer(GetOwningLocalPlayer(), true, suspendToken);
		}
	}
}
