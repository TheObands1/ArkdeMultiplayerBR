// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_GameModeBase.h"
#include "Core/BR_PlayerController.h"
#include "Core/BR_GameState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "BR_EffectZone.h"
#include "BR_EffectZoneSpawner.h"

void ABR_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	InitializePlayerStarts();
	SetupWorldSpawners();
}

void ABR_GameModeBase::PlayerIsKilled(AController* VictimController)
{
	ABR_PlayerController* MyVictimController = Cast<ABR_PlayerController>(VictimController);

	if (IsValid(MyVictimController))
	{
		MyVictimController->GameConclusion(false);
	}

	ABR_GameState* WorldGameState = GetWorld()->GetGameState<ABR_GameState>();

	if (IsValid(WorldGameState))
	{
		WorldGameState->CheckWinCondition();
	}
}

void ABR_GameModeBase::InitializePlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* CurrentPlayerStart = *It;
		if (IsValid(CurrentPlayerStart))
		{
			WorldPlayerStarts.Add(CurrentPlayerStart);
		}
	}
}

void ABR_GameModeBase::SetupWorldSpawners()
{
	int EffectZoneIndex = 0;

	for (TActorIterator<ABR_EffectZoneSpawner> It(GetWorld()); It; ++It)
	{
		ABR_EffectZoneSpawner* CurrentSpawner = *It;
		if (IsValid(CurrentSpawner))
		{
			CurrentSpawner->SetEffectZoneSubclass(SpawnableEffectZones[EffectZoneIndex % SpawnableEffectZones.Num()]);
			CurrentSpawner->RespawnEffectZone();
			EffectZoneIndex++;
		}
	}
}

AActor* ABR_GameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* BestStart = nullptr;

	int32 RandomIndex = FMath::RandHelper(WorldPlayerStarts.Num() - 1);

	if (RandomIndex >= 0 && WorldPlayerStarts.Num() > 0)
	{
		BestStart = WorldPlayerStarts[RandomIndex];
		WorldPlayerStarts.RemoveAt(RandomIndex);
	}

	if (IsValid(BestStart))
	{
		return BestStart;
	}
	else
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
}
