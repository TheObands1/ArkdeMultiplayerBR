// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BR_ANSLightingSlash.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"

void UBR_ANSLightingSlash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		//Verifies is CharacterActor is of class AAH_Character
		ABattleRoyaleCharacter* PossibleBattleRoyaleCharacter = Cast<ABattleRoyaleCharacter>(CharacterActor);
		if (IsValid(PossibleBattleRoyaleCharacter))
		{
			PossibleBattleRoyaleCharacter->SetMeleeRightFootComponentCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void UBR_ANSLightingSlash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		//Verifies is CharacterActor is of class AAH_Character
		ABattleRoyaleCharacter* PossibleBattleRoyaleCharacter = Cast<ABattleRoyaleCharacter>(CharacterActor);
		if (IsValid(PossibleBattleRoyaleCharacter))
		{
			PossibleBattleRoyaleCharacter->SetMeleeRightFootComponentCollision(ECollisionEnabled::NoCollision);
		}
	}
}
