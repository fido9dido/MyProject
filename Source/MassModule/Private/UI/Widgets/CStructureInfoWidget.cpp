#include "UI/Widgets/CStructureInfoWidget.h"
#include "MassEntitySubsystem.h"
#include "MassEntityManager.h"
#include "CommonTextBlock.h"
#include <Fragments/StructureFragment.h>
#include "DataAssets/CStructureDataAsset.h"
#include "Components/Button.h"
//#include <GameSettings/CGameSettings.h>
#include <Utilities/Util.h>
#include <MassCommonFragments.h>
#include "Subsystems/CWorkerSubsystem.h"
					
#define LOCTEXT_NAMESPACE "StructureText"

UCStructureInfoWidget::UCStructureInfoWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) 
, StructureData(nullptr)
{
	StructureData = nullptr;
}

void UCStructureInfoWidget::OnClicked(const FMassEntityHandle& structureHandle,const UCStructureDataAsset* structureData)
{
	if (!ensureMsgf(EntityManager, TEXT("EntityManager is null in UCStructureInfoWidget"))) { return; }
	
	StructureHandle = structureHandle;
	StructureData = structureData;
	StructureFragment = EntityManager->GetFragmentDataPtr<FStructureFragment>(StructureHandle);

	SynchronizeProperties();
}

void UCStructureInfoWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!StructureData || !StructureFragment) { return; }

	if (CurrentWorkersText)
	{
		CurrentWorkersText->SetText(FText::Format(LOCTEXT("StructureRuntime", "{0}"), FText::FromString(FString::Printf(TEXT("%02d / %02d"), StructureFragment->WorkersList.Num(), StructureFragment->MaxWorkers))));
	}

	if (NameText)
	{
		NameText->SetText(StructureData->Name);
	}	
	
	if (DescriptionText)
	{
		DescriptionText->SetText(StructureData->Description);
	}
}

void UCStructureInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMassEntitySubsystem* entitySubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	EntityManager = &entitySubsystem->GetMutableEntityManager();
	
	ExitButton->OnClicked.AddDynamic(this, &UCStructureInfoWidget::OnExitClick);
	AddWorkerButton->OnClicked.AddDynamic(this, &UCStructureInfoWidget::OnAddWorker);
	RemoveWorkerButton->OnClicked.AddDynamic(this, &UCStructureInfoWidget::OnRemoveWorker);
}

void UCStructureInfoWidget::OnExitClick()
{
	StructureFragment = nullptr;
	DeactivateWidget();
}

void UCStructureInfoWidget::OnAddWorker()
{
	ensure(GetWorld());

	if (!ensure(StructureHandle.IsValid())) { return; }

	UCWorkerSubsystem* workerSubsystem = GetGameInstance()->GetSubsystem<UCWorkerSubsystem>();
	workerSubsystem->AssignWorkerToStructure(StructureHandle);	

	SynchronizeProperties();
}

void UCStructureInfoWidget::OnRemoveWorker()
{
	/*if (!ensure(StructureHandle.IsValid())) { return; }
	UCWorkerSubsystem* workerSubsystem = GetGameInstance()->GetSubsystem<UCWorkerSubsystem>();
	workerSubsystem->RemoveWorkerFromStructure(StructureHandle);
	SynchronizeProperties();*/
}
