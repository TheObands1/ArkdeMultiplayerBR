// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_GameState.h"
#include "Core/BR_PlayerController.h"
#include "Core/BR_PlayerState.h"

void ABR_GameState::CheckWinCondition()
{
	int AlivePlayersCounter = 0;
	ABR_PlayerController* WinningPlayerController = nullptr;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABR_PlayerController* PlayerController = Cast<ABR_PlayerController>(It->Get());

		if (IsValid(PlayerController))
		{
			ABR_PlayerState* PlayerState = PlayerController->GetPlayerState<ABR_PlayerState>();

			if (IsValid(PlayerState))
			{
				if (PlayerState->IsAlive())
				{
					AlivePlayersCounter++;
					
					if (AlivePlayersCounter > 1)
					{
						return;
					}
					else
					{
						WinningPlayerController = PlayerController;
					}
				}
			}
		}
	}

	WinningPlayerController->GameConclusion(true);
}
