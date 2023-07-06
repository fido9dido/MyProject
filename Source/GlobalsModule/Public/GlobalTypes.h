// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ECLocomotionState : uint8
{
	  Idle
	, Walk
	, Jog
	, Jump
	, Count
};

enum class ECActorState : uint8
{
	  Patrol
	, Suspicious
	, Attack
	, Count
};