// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BR_EffectZone.generated.h"

class UCapsuleComponent;
class UGameplayEffect;
class UParticleSystemComponent;

UCLASS()
class BATTLEROYALE_API ABR_EffectZone : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CollisionCapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

public:
	TSubclassOf<UGameplayEffect> CurrentEffectOnPlayer;
	
public:	
	// Sets default values for this actor's properties
	ABR_EffectZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
