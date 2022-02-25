// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/BR_HUDHealthBar.h"
#include "GameplayAbilitySystem/BR_AttributeSet.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"

void UBR_HUDHealthBar::InitializeWidget()
{
	HealthPercent = 1.0f;
	if (IsValid(BattleRoyaleCharacterReference))
	{
		BattleRoyaleCharacterReference->OnCharacterHealthChangedDelegate.AddDynamic(this, &UBR_HUDHealthBar::UpdateHealth);
	}
}

void UBR_HUDHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
}
