// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/BR_HUDHealthBar.h"
#include "GameplayAbilitySystem/BR_AttributeSet.h"

void UBR_HUDHealthBar::InitializeWidget()
{
	UpdateHealth();
}

void UBR_HUDHealthBar::UpdateHealth()
{
	if (IsValid(CharacterAttributeSetReference))
	{
		HealthPercent = (CharacterAttributeSetReference->Health.GetCurrentValue()) / (CharacterAttributeSetReference->MaxHealth.GetCurrentValue());
		HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
	}
}
