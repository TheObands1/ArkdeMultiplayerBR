// Fill out your copyright notice in the Description page of Project Settings.


#include "BR_EffectZone.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameplayEffect.h"

// Sets default values
ABR_EffectZone::ABR_EffectZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CollisionCapsuleComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));

	bReplicates = true;

}

// Called when the game starts or when spawned
void ABR_EffectZone::BeginPlay()
{
	Super::BeginPlay();
	
}


