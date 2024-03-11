// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CPlaceholderWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor/WidgetCompilerLog.h"
#include "Misc/UObjectToken.h"
#include "GameFramework/PlayerState.h"
#include <GameSettings/CUISettings.h>
#include <Subsystems/CUIManagerSubsystem.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(CPlaceholderWidget)

#define LOCTEXT_NAMESPACE "UIExtension"

/////////////////////////////////////////////////////
// UCPlaceholderWidget

UCPlaceholderWidget::UCPlaceholderWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCPlaceholderWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Reset();

	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UCPlaceholderWidget::RebuildWidget()
{
	if (!IsDesignTime() && PlaceholderTag.IsValid())
	{
		Reset();
		Register();

	}

	if (IsDesignTime())
	{
		auto GetExtensionPointText = [this]()
		{
			return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "Extension Point\n{0}"), FText::FromName(PlaceholderTag.GetTagName()));
		};

		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);

		MessageBox->AddSlot()
			.Padding(5.0f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Center)
				.Text_Lambda(GetExtensionPointText)
			];

		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}

void UCPlaceholderWidget::Reset()
{
	ResetInternal();

	if(UGameInstance* gameInstance = GetGameInstance())
	{
		UCUIManagerSubsystem* uiManagerSubsystem = gameInstance->GetSubsystem<UCUIManagerSubsystem>();
		uiManagerSubsystem->RemovePlaceholderWidget(PlaceholderTag);
	}
}

void UCPlaceholderWidget::Register()
{
	OnAddOrRemoveWidget(ECPlaceholderAction::Add);

}

void UCPlaceholderWidget::OnAddOrRemoveWidget(ECPlaceholderAction action)
{
	UCUISettings* uiSettings = GetMutableDefault<UCUISettings>();
	TSubclassOf<UUserWidget> widgetClass = uiSettings->FindSlotOfWithTag(PlaceholderTag);
	if (!ensureMsgf(widgetClass, TEXT("PlaceHolder %s doesn't exist in UISettings in Project Settings"), *PlaceholderTag.GetTagName().ToString())) { return; }
	UCUIManagerSubsystem* uiManagerSubsystem = GetGameInstance()->GetSubsystem<UCUIManagerSubsystem>();

	if (action == ECPlaceholderAction::Add)
	{
		
		TObjectPtr<UCommonActivatableWidget> content= Cast<UCommonActivatableWidget>(CreateEntryInternal(widgetClass));
		if (content)
		{
			uiManagerSubsystem->AddPlaceholderWidget(PlaceholderTag, content);
		}
	}
	else
	{
		if (const TObjectPtr<UCommonActivatableWidget>& content = uiManagerSubsystem->FindPlaceholderWidget(PlaceholderTag))
		{
			RemoveEntryInternal(content);

		}
	}
}

#if WITH_EDITOR
void UCPlaceholderWidget::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (!PlaceholderTag.IsValid())
		{
			TSharedRef<FTokenizedMessage> Message = CompileLog.Error(FText::Format(LOCTEXT("UCPlaceholderWidget_NoTag", "{0} has no PlaceholderTag specified - Tags are required to locate and place widgets into the intended locations."), FText::FromString(GetName())));
			Message->AddToken(FUObjectToken::Create(this));
		}
	}
}
#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

