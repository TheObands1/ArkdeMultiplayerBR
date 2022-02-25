// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BR_EffectZoneSpawner.generated.h"

class ABR_EffectZone;

UCLASS()
class BATTLEROYALE_API ABR_EffectZoneSpawner : public AActor
{
	GENERATED_BODY()

protected:
	FTimerHandle Respawn_TimerHandle;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float RespawnTime;

	TSubclassOf<ABR_EffectZone> EffectSubclass;
	
public:	
	// Sets default values for this actor's properties
	ABR_EffectZoneSpawner();

	UFUNCTION(BlueprintCallable)
	void SetupRespawnTimer();
	
	UFUNCTION()
	void RespawnEffectZone();

	void SetEffectZoneSubclass(TSubclassOf<ABR_EffectZone> SelectedEffectZone);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
