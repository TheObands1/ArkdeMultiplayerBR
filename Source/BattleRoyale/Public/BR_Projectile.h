// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BR_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;
class ABattleRoyaleCharacter;

UCLASS()
class BATTLEROYALE_API ABR_Projectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplosionEffect;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Over Time")
	bool DoesProjectileApplyDoT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* DestroyingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* ProjectileSound;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Range")
	float Range;

public:	
	// Sets default values for this actor's properties
	ABR_Projectile();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Multicast_IgnoreActor(ABattleRoyaleCharacter* CharacterToIgnore);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void DetectOverlappingCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ApplyGameplayEffectToHitActor(AActor* HittedActor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ApplyDamageOverTimeToHitActor(AActor* HittedActor);
	
	void PlayDestructionSound(FVector SoundLocation);

	void PlayProjectileSound();

	void DestroyProjectile(FVector DestroyLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitializeProjectileMovement(FVector Velocity);


};
