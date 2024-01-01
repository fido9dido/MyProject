// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkeletalVisualization.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCSkeletalVisualization : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSkeletalVisualization();

};
