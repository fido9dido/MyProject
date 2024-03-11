// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/CUISettings.h"

#if WITH_EDITOR
void UCUISettings::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

}
#endif // WITH_EDITOR


TSubclassOf<UCommonUserWidget> UCUISettings::GetRootLayoutClass() const
{
	return RootLayoutWidgetClass.LoadSynchronous();
}

TSubclassOf<UUserWidget> UCUISettings::FindSlotOfWithTag(FGameplayTag slotTag)
{
	for (TSoftObjectPtr<UCPlaceholderSlotDefinition>& definition : PlaceholderSlotDefinition)
	{
		UCPlaceholderSlotDefinition* slot = definition.LoadSynchronous();
		if (!slot || slot->Widget.IsNull() || slot->SlotTag != slotTag) { continue; }

		return slot->Widget.LoadSynchronous();

	}
	return nullptr;
}
