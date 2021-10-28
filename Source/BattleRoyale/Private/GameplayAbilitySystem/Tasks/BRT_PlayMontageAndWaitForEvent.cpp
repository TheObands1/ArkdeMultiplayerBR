// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Tasks/BRT_PlayMontageAndWaitForEvent.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemGlobals.h"

UBRT_PlayMontageAndWaitForEvent::UBRT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	RateVar = 1.0f;
	bStopWhenAbilityEndsVar = true;
}

UBRT_PlayMontageAndWaitForEvent* UBRT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
	UBRT_PlayMontageAndWaitForEvent* myObj = NewAbilityTask<UBRT_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);

	myObj->MontageToPlayVar = MontageToPlay;
	myObj->EventTagsVar = EventTags;
	myObj->RateVar = Rate;
	myObj->StartSectionVar = StartSection;
	myObj->AnimRootMotionTranslationScaleVar = AnimRootMotionTranslationScale;
	myObj->bStopWhenAbilityEndsVar = bStopWhenAbilityEnds;

	return myObj;
}

void UBRT_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bHasMontageAlreadyBeenPlayed = false;

	if (AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

		if (ActorInfo)
		{
			UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

			if (AnimInstance != nullptr)
			{
				EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTagsVar, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UBRT_PlayMontageAndWaitForEvent::OnGameplayEvent));

				if ((AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlayVar, RateVar, StartSectionVar)) > 0.0f)
				{
					if (ShouldBroadcastAbilityTaskDelegates() == false)
					{
						return;
					}

					CanceledHanlde = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UBRT_PlayMontageAndWaitForEvent::OnAbilityCanceled);
					BlendingOutDelegate.BindUObject(this, &UBRT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
					AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlayVar);

					MontageEndedDelegate.BindUObject(this, &UBRT_PlayMontageAndWaitForEvent::OnMontageEnded);
					AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlayVar);

					ACharacter* ProbableCharacter = Cast<ACharacter>(GetAvatarActor());

					if (ProbableCharacter && (ProbableCharacter->GetLocalRole() == ROLE_Authority || ProbableCharacter->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
					{
						ProbableCharacter->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScaleVar);
					}

					bHasMontageAlreadyBeenPlayed = true;
				}
			}
		}
	}

	if (!bHasMontageAlreadyBeenPlayed)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCanceledDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UBRT_PlayMontageAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCanceled();

	Super::ExternalCancel();
}

FString UBRT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;

	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlayVar) ? MontageToPlayVar : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s (CurrentPlayingMontage): %s"), *GetNameSafe(MontageToPlayVar), *GetNameSafe(PlayingMontage));
}

void UBRT_PlayMontageAndWaitForEvent::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CanceledHanlde);
		if (bInOwnerFinished && bStopWhenAbilityEndsVar)
		{
			StopPlayingMontage();
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTagsVar, EventHandle);

	}

	Super::OnDestroy(bInOwnerFinished);
}

void UBRT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlayVar)
	{
		if (Montage == MontageToPlayVar)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			ACharacter* PossibleCharacter = Cast<ACharacter>(GetAvatarActor());
			if (PossibleCharacter && (PossibleCharacter->GetLocalRole() == ROLE_Authority || (PossibleCharacter->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				PossibleCharacter->SetAnimRootMotionTranslationScale(1.f);
			}

		}
	}

	if (bIsInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrumpedDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOutDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UBRT_PlayMontageAndWaitForEvent::OnAbilityCanceled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCanceledDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UBRT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (!bIsInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleteDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

void UBRT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData tempData = *Payload;
		tempData.EventTag = EventTag;

		OnEventReceivedDelegate.Broadcast(EventTag, tempData);
	}
}

bool UBRT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlayVar)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlayVar);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();

			return true;
		}
	}

	return false;
}
