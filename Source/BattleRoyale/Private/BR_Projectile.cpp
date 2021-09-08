// Fill out your copyright notice in the Description page of Project Settings.


#include "BR_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABR_Projectile::ABR_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

}

// Called when the game starts or when spawned
void ABR_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABR_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

