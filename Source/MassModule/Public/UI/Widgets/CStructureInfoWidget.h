// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MassEntityTypes.h"
#include "CStructureInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASSMODULE_API UCStructureInfoWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()	

public:
	UCStructureInfoWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> AddWorkerButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> RemoveWorkerButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCommonTextBlock> CurrentWorkersText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCommonTextBlock> NameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCommonTextBlock> DescriptionText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

protected:
	struct FMassEntityManager* EntityManager;
	
	FMassEntityHandle StructureHandle;
	
	const class UCStructureDataAsset* StructureData;

	struct FStructureFragment* StructureFragment;
public:	  
	UFUNCTION()
	void OnClicked(const FMassEntityHandle& structureHandle, const class UCStructureDataAsset* structureData);
	//~ Begin UWidget
	virtual void SynchronizeProperties() override;
	//~ End UWidget
protected:	
	//~ Begin UUserWidget 
	virtual void NativeConstruct() override; 
	//~ End UUserWidget

	UFUNCTION()
	void OnExitClick();	 	
	UFUNCTION()
	void OnAddWorker();
	UFUNCTION()
	void OnRemoveWorker();
};
