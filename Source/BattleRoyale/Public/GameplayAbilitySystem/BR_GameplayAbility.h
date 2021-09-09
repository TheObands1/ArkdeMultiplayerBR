// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BattleRoyale/BattleRoyale.h"
#include "BR_GameplayAbility.generated.h"

class ABattleRoyaleCharacter;

/**
 * 
 */
UCLASS()
class BATTLEROYALE_API UBR_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBR_GameplayAbility();

public:
	//Ability Input ID's
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability")
	EBR_AbilityInputID AbilityInputID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability")
	EBR_AbilityInputID AbilityID;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	ABattleRoyaleCharacter* BattleRoyaleCharacterReference;
	*/

protected:
	/** Actually activate ability, do not call this directly */
	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	//virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr);

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	//virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

};
