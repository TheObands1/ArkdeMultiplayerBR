// Fill out your copyright notice in the Description page of Project Settings.


#include "BR_EffectZoneSpawner.h"
#include "BR_EffectZone.h"

// Sets default values
ABR_EffectZoneSpawner::ABR_EffectZoneSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RespawnTime = 15;

}
// Called when the game starts or when spawned
void ABR_EffectZoneSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABR_EffectZoneSpawner::SetupRespawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(Respawn_TimerHandle, this, &ABR_EffectZoneSpawner::RespawnEffectZone, RespawnTime, false);
}

void ABR_EffectZoneSpawner::RespawnEffectZone()
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;

	FTransform SpawnTrasform = GetActorTransform();
	FVector SpawnLocation = SpawnTrasform.GetLocation(); // +FVector
	SpawnTrasform.SetLocation(SpawnLocation);

	GetWorld()->SpawnActor<ABR_EffectZone>(EffectSubclass, SpawnTrasform, SpawnParams);

}

void ABR_EffectZoneSpawner::SetEffectZoneSubclass(TSubclassOf<ABR_EffectZone> SelectedEffectZone)
{
	EffectSubclass = SelectedEffectZone;
}

