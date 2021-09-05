// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable)
enum class EBR_AbilityInputID :uint8
{
	//Input 0
	None UMETA(DisplayName = "None"),
	//Input 1
	Confirm  UMETA(DisplayName = "Confirm"),
	//Input 2
	Cancel  UMETA(DisplayName = "Cancel"),
	//Input 3
	Sprint  UMETA(DisplayName = "Sprint"),
	//Input 4
	Jump  UMETA(DisplayName = "Jump"),
	//Input 5
	Ability1  UMETA(DisplayName = "Ability1"),
	//Input 6
	Ability2  UMETA(DisplayName = "Ability2"),

};
