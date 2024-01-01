// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ECLocomotionState : uint8
{
	  Idle
	, Walk
	, Jog
	, Jump
	, Gather
	, Construction
	, Count
};

enum class ECActorState : uint8
{
	  Patrol
	, Suspicious
	, Attack
	, Count
};

UENUM(BlueprintType)
enum class ECResourceType : uint8
{
	Tree
	, Stone
	//, Water
	, Count
	, Unset
	, All
};

UENUM(BlueprintType)
enum class ECWorkerType : uint8
{
	Unset
	, Construction
	, Gatherer
	, Count
};