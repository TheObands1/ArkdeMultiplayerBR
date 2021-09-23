// Fill out your copyright notice in the Description page of Project Settings.


#include "BR_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"

// Sets default values
ABR_Projectile::ABR_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionDetectorComponent"));
	RootComponent = CollisionDetectorComponent;
	CollisionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionDetectorComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionDetectorComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

}

// Called when the game starts or when spawned
void ABR_Projectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ABR_Projectile::DetectOverlappingCollision);
	CollisionDetectorComponent->OnComponentHit.AddDynamic(this, &ABR_Projectile::DetectCollisionHit);
	
}

void ABR_Projectile::DetectOverlappingCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != this)
	{
		ABattleRoyaleCharacter* PossibleBattleRoyaleCharacter = Cast<ABattleRoyaleCharacter>(OtherActor);
		if (IsValid(PossibleBattleRoyaleCharacter))
		{
			if (PossibleBattleRoyaleCharacter != GetInstigator())
			{
				BP_ApplyGameplayEffectToHitActor(PossibleBattleRoyaleCharacter);
				DestroyProjectile(SweepResult.ImpactPoint);
			}
		}
	}
}

void ABR_Projectile::DetectCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DestroyProjectile(Hit.ImpactPoint);
}

void ABR_Projectile::PlayDestructionSound(FVector SoundLocation)
{
	if (!IsValid(DestroyingSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroyingSound, SoundLocation);
}

void ABR_Projectile::PlayProjectileSound()
{
	if (!IsValid(ProjectileSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSound, GetActorLocation());
}

void ABR_Projectile::DestroyProjectile(FVector DestroyLocation)
{
	PlayDestructionSound(DestroyLocation);
	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, DestroyLocation);
	}
	Destroy();
}

// Called every frame
void ABR_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABR_Projectile::InitializeProjectileMovement(FVector Velocity)
{
	ProjectileMovementComponent->Velocity = Velocity;
}

