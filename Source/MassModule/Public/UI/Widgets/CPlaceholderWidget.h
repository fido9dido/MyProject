// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/DynamicEntryBoxBase.h"
#include "GameplayTagContainer.h"

#include "CPlaceholderWidget.generated.h"

UENUM()
enum class ECPlaceholderAction : uint8
{
	  Add
	, Remove
	, Count
};

/**
 * A slot that defines a location in a layout, where content can be added later
 */
UCLASS()
class MASSMODULE_API UCPlaceholderWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(TSubclassOf<UUserWidget>, FOnGetWidgetClassForData, UObject*, DataItem);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnConfigureWidgetForData, UUserWidget*, Widget, UObject*, DataItem);

protected:
	/** The tag that defines this placeholder point */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeholder")
	FGameplayTag PlaceholderTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeholder", meta = (IsBindableEvent = "True"))
	FOnGetWidgetClassForData GetWidgetClassForData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeholder", meta = (IsBindableEvent = "True"))
	FOnConfigureWidgetForData ConfigureWidgetForData;

public:

	UCPlaceholderWidget(const FObjectInitializer& ObjectInitializer);

	//~UWidget interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~End of UWidget interface

private:
	void Reset();
	void Register();
	void OnAddOrRemoveWidget(ECPlaceholderAction action);
};
