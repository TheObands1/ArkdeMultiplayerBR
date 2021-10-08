// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "BR_PlayerState.generated.h"

/**
 * 
 */

class UBR_AttributeSet;
class UBR_AbilitySystemInterface;
class UBR_GameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class BATTLEROYALE_API ABR_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// -- Gameplay Ability System elements --

public:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

public:
	//References/Variables
	UPROPERTY()
	UBR_AttributeSet* AttributeSet;

public:
	//Functions

	ABR_PlayerState();

	virtual void BeginPlay() override;

public:

	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UBR_AttributeSet* GetAttributeSet() const;

	bool IsAlive() const;

	float GetHealth() const;

	float GetMaxHealth() const;

	float GetHealthRegen() const;

public: 
	// Delegates;
	FDelegateHandle HealthChangedDelegateHandle;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	
};
