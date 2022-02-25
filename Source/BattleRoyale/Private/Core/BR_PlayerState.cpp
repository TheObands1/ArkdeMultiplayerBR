// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_PlayerState.h"
#include "GameplayAbilitySystem/BR_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"
#include "GameplayEffectExtension.h"

ABR_PlayerState::ABR_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UBR_AttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;
}

void ABR_PlayerState::BeginPlay()
{
	if (IsValid(AbilitySystemComponent))
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ABR_PlayerState::OnHealthChanged);
	}
}

UAbilitySystemComponent* ABR_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBR_AttributeSet* ABR_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool ABR_PlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABR_PlayerState::GetHealth() const
{
	return GetAttributeSet()->GetHealth();
}

float ABR_PlayerState::GetMaxHealth() const
{
	return GetAttributeSet()->GetMaxHealth();
}

float ABR_PlayerState::GetHealthRegen() const
{
	return GetAttributeSet()->GetHealthRegen();
}

void ABR_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (IsValid(AbilitySystemComponent) && !IsAlive() && GetLocalRole() == ROLE_Authority)
	{
		ABattleRoyaleCharacter* PossibleBRCharacter = Cast<ABattleRoyaleCharacter>(GetPawn());

		if (IsValid(PossibleBRCharacter))
		{
			AActor* KillerActor = Data.GEModData->EffectSpec.GetEffectContext().GetEffectCauser();
			if (IsValid(KillerActor))
			{
				ABattleRoyaleCharacter* KillerCharacter = Cast<ABattleRoyaleCharacter>(KillerActor);
				if (IsValid(KillerCharacter))
				{
					PossibleBRCharacter->Server_Die(KillerCharacter);
				}
			}
		}
	}
}
