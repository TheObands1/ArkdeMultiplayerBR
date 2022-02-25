// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/BRGA_Firebolt.h"
#include "GameplayAbilitySystem/Tasks/BRT_PlayMontageAndWaitForEvent.h"
#include "BattleRoyale/BattleRoyaleCharacter.h"
#include "BattleRoyale/Public/BR_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"


UBRGA_Firebolt::UBRGA_Firebolt()
{
}

void UBRGA_Firebolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UBRT_PlayMontageAndWaitForEvent* MontageTask = UBRT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, CastMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f);

	MontageTask->OnBlendOutDelegate.AddDynamic(this, &UBRGA_Firebolt::OnMontageCompleted);
	MontageTask->OnCompleteDelegate.AddDynamic(this, &UBRGA_Firebolt::OnMontageCompleted);

	MontageTask->OnInterrumpedDelegate.AddDynamic(this, &UBRGA_Firebolt::OnMontageCancelled);
	MontageTask->OnCanceledDelegate.AddDynamic(this, &UBRGA_Firebolt::OnMontageCancelled);

	MontageTask->OnEventReceivedDelegate.AddDynamic(this, &UBRGA_Firebolt::EventReceived);

	MontageTask->ReadyForActivation();
}

void UBRGA_Firebolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UBRGA_Firebolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBRGA_Firebolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		ABattleRoyaleCharacter* PossibleCharacter = Cast<ABattleRoyaleCharacter>(GetAvatarActorFromActorInfo());

		if (!IsValid(PossibleCharacter))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		

		FVector StartLocation = PossibleCharacter->GetMesh()->GetSocketLocation(AbilitySocketName);
		FVector EndLocation = PossibleCharacter->GetCameraBoom()->GetComponentLocation() + (PossibleCharacter->GetFollowCamera()->GetForwardVector()  * ProjectileRange);
		FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

		FTransform SpawnTransform = PossibleCharacter->GetMesh()->GetSocketTransform(AbilitySocketName);
		SpawnTransform.SetRotation(ProjectileRotation.Quaternion());
		SpawnTransform.SetScale3D(FVector::OneVector);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABR_Projectile* Firebolt = GetWorld()->SpawnActorDeferred<ABR_Projectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), PossibleCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Firebolt->Multicast_IgnoreActor(PossibleCharacter);
		Firebolt->Range = ProjectileRange;

		Firebolt->CollisionDetectorComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		Firebolt->FinishSpawning(SpawnTransform);
	}
}
