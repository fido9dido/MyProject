// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Clusters/CNPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Fragments/ClusterVisualizationFragment.h"
#include "UObject/ObjectSaveContext.h"
#include "MassAgentComponent.h"
#include "Systems/CClusterSettings.h"
#include "MassMovementFragments.h"
#include <Components/CapsuleComponent.h>
#include <MassEntityConfigAsset.h>
#include <MassLookAtFragments.h>

ACNPCCharacter::ACNPCCharacter(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	AIControllerClass = nullptr;

	AgentComponent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgentComponent"));

	SetupSkeletalMeshes();
}

void ACNPCCharacter::PostInitProperties()
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

		GetCharacterMovement()->bOrientRotationToMovement = true;

	}
}

void ACNPCCharacter::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	if (!HasAnyFlags(RF_ClassDefaultObject) && bShouldBuildOnConstruct)
	{
		BuildCharacter();
	}
}

#if WITH_EDITOR
void ACNPCCharacter::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	if (FProperty* memberProperty = propertyChangedEvent.MemberProperty)
	{
		const FName memberPropertyName = memberProperty->GetFName();

		if (memberPropertyName == GET_MEMBER_NAME_CHECKED(ACNPCCharacter, ClusterCharacterData))
		{
			BuildCharacter();
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void ACNPCCharacter::PreSave(FObjectPreSaveContext objectSaveContext)
{
	USkeletalMeshComponent* BaseMesh = GetMesh();
	if (IsValid(BaseMesh))
	{
		BaseMesh->SetSkeletalMesh(nullptr);
		for (int MaterialIdx = 0; MaterialIdx < BaseMesh->GetNumMaterials(); ++MaterialIdx)
		{
			BaseMesh->SetMaterial(MaterialIdx, nullptr);
		}
		BaseMesh->EmptyOverrideMaterials();
	}

	Super::PreSave(objectSaveContext);
}

void ACNPCCharacter::BuildCharacter()
{
	if(ClusterCharacterData) 
	{
		FClusterCharacterDefinition characterDefinition = ClusterCharacterData->GetCharacterDefinition();
	
		BuildCharacterFromDefinition(characterDefinition);
	}
}

void ACNPCCharacter::BuildCharacterFromDefinition(const FClusterCharacterDefinition& inCharacterDefinition)
{
	// Adjust Capsule dimensions
	if (UCapsuleComponent* capsule = GetCapsuleComponent())
	{
		float CapsuleHeight = inCharacterDefinition.MeshDefinition.CapsuleHeight;
		float CapsuleWidth = inCharacterDefinition.MeshDefinition.CapsuleWidth;
		capsule->SetCapsuleHalfHeight(CapsuleHeight == 0 ? 88.f : CapsuleHeight);
		capsule->SetCapsuleRadius(CapsuleWidth == 0 ? 34.f : CapsuleWidth);
	}

	UpdateMeshes(inCharacterDefinition);
	
	// Anim Instance is not automatically reinitialized which can lead to garbage poses
	// so we manually force the initialize to prevent this.
	if (USkeletalMeshComponent* baseMesh = GetSkeletalMeshComponent())
	{
		baseMesh->InitAnim(true);
		baseMesh->PrimaryComponentTick.bCanEverTick = false;
		baseMesh->SetComponentTickEnabled(false);
	}
}

void ACNPCCharacter::SetupSkeletalMeshes()
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

void ACNPCCharacter::UpdateMeshes(const FClusterCharacterDefinition& characterDefinition)
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

void ACNPCCharacter::UpdateSkeletalMesh(USkeletalMeshComponent* skeletalMeshComponent, TSoftObjectPtr<USkeletalMesh> softSkeletalMeshPtr)
{
	USkeletalMesh* skeletalMesh = softSkeletalMeshPtr.LoadSynchronous();

	if (skeletalMesh && ClusterCharacterData && ClusterCharacterData->RayTracingMinLOD >= 0)
	{
		skeletalMesh->SetRayTracingMinLOD(ClusterCharacterData->RayTracingMinLOD);
	}

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

USkeletalMeshComponent* ACNPCCharacter::GetSkeletalMeshComponent()
{	
	return GetMesh();
}

UDataAsset* ACNPCCharacter::GetCurrentLocomotionAnimSet() const
{
	return CharacterDefinition.LocomotionAnimSet.LoadSynchronous();
}

void ACNPCCharacter::OnGetOrSpawn(FMassEntityManager* entitySubsystem, const FMassEntityHandle massAgent)
{
	if (entitySubsystem && entitySubsystem->IsEntityActive(massAgent))
	{
		if (FMassVelocityFragment* massVelocityFragment = entitySubsystem->GetFragmentDataPtr<FMassVelocityFragment>(massAgent))
		{
			if (UCharacterMovementComponent* charMovement = GetCharacterMovement())
			{
				charMovement->Velocity = massVelocityFragment->Value;
			}
		}

		FClusterVisualizationFragment* visualizationFragment = entitySubsystem->GetFragmentDataPtr<FClusterVisualizationFragment>(massAgent);
		if (visualizationFragment)
		{
			BuildCharacter();
		}
	}
}
