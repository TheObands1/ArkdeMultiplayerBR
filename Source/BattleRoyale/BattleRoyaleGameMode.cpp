// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleRoyaleGameMode.h"
#include "BattleRoyaleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABattleRoyaleGameMode::ABattleRoyaleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
