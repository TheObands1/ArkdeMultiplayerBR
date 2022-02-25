// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BR_GameState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEROYALE_API ABR_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void CheckWinCondition();
	
};
