// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/BR_GameplayAbility.h"
#include "BRGA_Firebolt.generated.h"

/**
 * 
 */

class UAnimMontage;
class ABR_Projectile;


UCLASS()
class BATTLEROYALE_API UBRGA_Firebolt : public UBR_GameplayAbility
{
	GENERATED_BODY()

public:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FGameplayTag ProjectileSpawnTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FName AbilitySocketName;

public:
	UBRGA_Firebolt();

public: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* CastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<ABR_Projectile> ProjectileClass;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
