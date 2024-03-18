// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CAgentCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Fragments/ClusterVisualizationFragment.h"
#include "UObject/ObjectSaveContext.h"
#include "MassAgentComponent.h"
#include "GameSettings/CClusterSettings.h"
#include "Fragments/ClusterVisualizationFragment.h"
#include "MassMovementFragments.h"
#include <Components/CapsuleComponent.h>
#include <MassEntityConfigAsset.h>
#include <MassLookAtFragments.h>

ACAgentCharacter::ACAgentCharacter(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	AIControllerClass = nullptr;

	AgentComponent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgentComponent"));

	SetupSkeletalMeshes();
}

void ACAgentCharacter::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject) && ensure(AgentComponent))
	{
		if (const UCClusterSettings* settings = UCClusterSettings::Get())
		{
			if (UMassEntityConfigAsset* assetInstance = settings->GetAgentConfig())
			{
				AgentComponent->SetEntityConfig(FMassEntityConfig(*assetInstance));
			}
		}
	}

	if (UCharacterMovementComponent* characterMovement = GetCharacterMovement())
	{
		characterMovement->bOrientRotationToMovement = true;
	}
}

void ACAgentCharacter::PreSave(FObjectPreSaveContext objectSaveContext)
{
	USkeletalMeshComponent* BaseMesh = GetMesh();
	if (IsValid(BaseMesh))
	{
		BaseMesh->SetSkeletalMesh(nullptr);
		for (int32 MaterialIdx = 0; MaterialIdx < BaseMesh->GetNumMaterials(); ++MaterialIdx)
		{
			BaseMesh->SetMaterial(MaterialIdx, nullptr);
		}
		BaseMesh->EmptyOverrideMaterials();
	}

	Super::PreSave(objectSaveContext);
}

void ACAgentCharacter::BuildCharacterFromDefinition(const FClusterCharacterDefinition& characterDefinition)
{
	// Adjust Capsule dimensions
	if (UCapsuleComponent* capsule = GetCapsuleComponent())
	{
		float capsuleHeight = characterDefinition.MeshDefinition.CapsuleHeight;
		float capsuleWidth = characterDefinition.MeshDefinition.CapsuleWidth;
		capsule->SetCapsuleHalfHeight(capsuleHeight == 0 ? 88.f : capsuleHeight);
		capsule->SetCapsuleRadius(capsuleWidth == 0 ? 34.f : capsuleWidth);
	}

	SetSkeletalMesh(characterDefinition);
	
	// Anim Instance is not automatically reinitialized which can lead to garbage poses
	// so we manually force the initialize to prevent this.
	if (USkeletalMeshComponent* baseMesh = GetSkeletalMeshComponent())
	{
		baseMesh->InitAnim(true);
		baseMesh->PrimaryComponentTick.bCanEverTick = false;
		baseMesh->SetComponentTickEnabled(false);
	}
}

void ACAgentCharacter::SetupSkeletalMeshes()
{
	USkeletalMeshComponent* baseMeshComponent = GetMesh();

	if (baseMeshComponent)
	{
		// Lower the base mesh and rotate it to align with capsule component
		baseMeshComponent->SetRelativeTransform(FTransform(
			FRotator(0, 90, 0),
			FVector(0, 0, -90)
		));
	}
}

void ACAgentCharacter::SetSkeletalMesh(const FClusterCharacterDefinition& characterDefinition)
{
	// Update the Skeletal Meshes
	USkeletalMeshComponent* meshComponent = GetSkeletalMeshComponent();

	if (meshComponent && characterDefinition.MeshDefinition.Mesh)
	{
		UpdateSkeletalMesh(meshComponent, characterDefinition.MeshDefinition.Mesh);

		meshComponent->SetRelativeScale3D(FVector(characterDefinition.ScaleFactor));

		meshComponent->SetLeaderPoseComponent(nullptr);
	}
}

void ACAgentCharacter::UpdateSkeletalMesh(USkeletalMeshComponent* skeletalMeshComponent, TSoftObjectPtr<USkeletalMesh> softSkeletalMeshPtr)
{
	USkeletalMesh* skeletalMesh = softSkeletalMeshPtr.LoadSynchronous();

	UWorld* world = skeletalMeshComponent->GetWorld();
	if (skeletalMeshComponent->bUseRefPoseOnInitAnim && world)
	{
		EVisibilityBasedAnimTickOption initialVisibilityBasedAnimTickOption = skeletalMeshComponent->VisibilityBasedAnimTickOption;
		skeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);

		TWeakObjectPtr<USkeletalMeshComponent> weakSkeletalMeshComponent = skeletalMeshComponent;
		world->GetTimerManager().SetTimerForNextTick([weakSkeletalMeshComponent, initialVisibilityBasedAnimTickOption]()
			{
				if (USkeletalMeshComponent* skeletalMeshComponentRef = weakSkeletalMeshComponent.Get())
				{
					// After the first tick, go back to the original setting
					skeletalMeshComponentRef->VisibilityBasedAnimTickOption = initialVisibilityBasedAnimTickOption;
				}
			});
	}
	else
	{
		skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	}
}

USkeletalMeshComponent* ACAgentCharacter::GetSkeletalMeshComponent()
{	
	return GetMesh();
}

void ACAgentCharacter::OnGetOrSpawn(FMassEntityManager& entityManager, const FMassEntityHandle massAgent)
{
	if (entityManager.IsEntityActive(massAgent))
	{
		if (FMassVelocityFragment* massVelocityFragment = entityManager.GetFragmentDataPtr<FMassVelocityFragment>(massAgent))
		{
			if (UCharacterMovementComponent* charMovement = GetCharacterMovement())
			{
				charMovement->Velocity = massVelocityFragment->Value;
			}
		}			  
		
		// I implemented GetConstSharedFragment in entity Subsystem
		const FClusterDataAssetSharedFragment* clusterDataAssetSharedFragment = entityManager.GetConstSharedFragmentDataPtr<FClusterDataAssetSharedFragment>(massAgent);
		FClusterVisualizationFragment* visualizationFragment = entityManager.GetFragmentDataPtr<FClusterVisualizationFragment>(massAgent);
		
		if (visualizationFragment && 
			clusterDataAssetSharedFragment &&
			clusterDataAssetSharedFragment->ClusterDefinitionData &&
			clusterDataAssetSharedFragment->ClusterDefinitionData->CharacterDefinitionList.IsValidIndex(visualizationFragment->CharacterDefinitionID))
		{
			BuildCharacterFromDefinition(clusterDataAssetSharedFragment->ClusterDefinitionData->CharacterDefinitionList[visualizationFragment->CharacterDefinitionID]);
		}
	}
}
