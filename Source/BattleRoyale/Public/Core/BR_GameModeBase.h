// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BR_GameModeBase.generated.h"

/**
 * 
 */

class AController;
class APlayerStart;
class ABR_EffectZone;

UCLASS()
class BATTLEROYALE_API ABR_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	TArray<APlayerStart*> WorldPlayerStarts;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<ABR_EffectZone>> SpawnableEffectZones;

protected:

	virtual void BeginPlay() override;

public:

	void PlayerIsKilled(AController* VictimController);

	void InitializePlayerStarts();

	void SetupWorldSpawners();

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
};
