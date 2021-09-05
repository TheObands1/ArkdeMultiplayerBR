// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BattleRoyale/BattleRoyale.h"
#include "BR_GameplayAbility.generated.h"

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
	
};
