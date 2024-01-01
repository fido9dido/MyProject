// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/CUIManagerSubsystem.h"
#include <GameSettings/CUISettings.h>
#include <CommonUserWidget.h>
#include "LogTypes.h"
#include "UI/Widgets/CRootLayout.h"
#include <Kismet/GameplayStatics.h>

void UCUIManagerSubsystem::OnBeginPlay(UWorld& world)
{
	UCUISettings* uiSettings = GetMutableDefault<UCUISettings>();
	TSubclassOf<UCommonUserWidget> layoutClass = uiSettings->GetRootLayoutClass();
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ULocalPlayer* localPlayer = playerController ? playerController->GetLocalPlayer() : nullptr;

	if (!RootLayout && uiSettings && localPlayer)
	{
		CreateLayoutWidget(localPlayer, layoutClass);
	}
	
	for (TSoftObjectPtr<UCWidgetDefinition>& widgetEntry: uiSettings->WidgetDefinition)
	{
		const UCWidgetDefinition* widgetDefinition = widgetEntry.LoadSynchronous();
		RootLayout->AddWidgetToLayerStack(widgetDefinition->LayerTag, widgetDefinition->Widget.LoadSynchronous());
	}
}

void UCUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SetRootLayout(nullptr);
	
}

const UCRootLayout* UCUIManagerSubsystem::GetRootLayout() const
{
	return RootLayout;
}

UCRootLayout* UCUIManagerSubsystem::GetRootLayout()
{
	return RootLayout;
}

void UCUIManagerSubsystem::PushStreamedContentToLayer(FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	const bool bSuspendInputUntilComplete = true;
	RootLayout->AddWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
}

void UCUIManagerSubsystem::PopContentFromLayer(UCommonActivatableWidget* activatableWidget)
{
	if (!activatableWidget)
	{
		// Ignore request to pop an already deleted widget
		return;
	}

	RootLayout->FindAndRemoveWidgetFromLayer(activatableWidget);
}

void UCUIManagerSubsystem::SetRootLayout(UCRootLayout* layout)
{
	if (RootLayout == layout) { return; }

	RootLayout = layout;
}

void UCUIManagerSubsystem::AddSlotWidget(FGameplayTag slotTag, TObjectPtr<UCommonActivatableWidget>& entryWidget)
{
	if (!slotTag.IsValid() || !entryWidget) { return; }

	PlaceholderMap.Emplace(slotTag, entryWidget);

}

void UCUIManagerSubsystem::RemovePlaceholderWidget(FGameplayTag& slotTag)
{
	PlaceholderMap.Remove(slotTag);
}

TObjectPtr<UCommonActivatableWidget>* UCUIManagerSubsystem::GetPlaceholderWidget(FGameplayTag& slotTag)
{
	return PlaceholderMap.Find(slotTag);
}

void UCUIManagerSubsystem::CreateLayoutWidget(ULocalPlayer* localPlayer, TSubclassOf<UCommonUserWidget> layoutWidgetClass)
{
	if (APlayerController* PlayerController = localPlayer->GetPlayerController(GetWorld()))
	{
		if (ensure(layoutWidgetClass && !layoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UCRootLayout* newLayoutObject = CreateWidget<UCRootLayout>(PlayerController, layoutWidgetClass);
			SetRootLayout(newLayoutObject);

			AddLayoutToViewport(localPlayer, newLayoutObject);
		}
	}
}

void UCUIManagerSubsystem::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UCRootLayout* Layout)
{
	UE_LOG(LogCUI, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UCUIManagerSubsystem::OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UCRootLayout* Layout)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
}

