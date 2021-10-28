// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "BRT_PlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBRPlayMontageAndWaitForEventSignature, FGameplayTag, EventTag, FGameplayEventData, EventData);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBRPlayMontageAndWaitForEventSignature, FGameplayTag, EventTag, FGameplayEventData, EventData);

/**
 * 
 */

class UAnimMontage;

UCLASS()
class BATTLEROYALE_API UBRT_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()


private:
	UPROPERTY()
	UAnimMontage* MontageToPlayVar;

	UPROPERTY()
	FGameplayTagContainer EventTagsVar;

	UPROPERTY()
	float RateVar;

	UPROPERTY()
	FName StartSectionVar;

	UPROPERTY()
	bool bStopWhenAbilityEndsVar;

	UPROPERTY()
	float AnimRootMotionTranslationScaleVar;

public:

	FOnMontageBlendingOutStarted BlendingOutDelegate;

	FOnMontageEnded MontageEndedDelegate;

	FDelegateHandle CanceledHanlde;

	FDelegateHandle EventHandle;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnBRPlayMontageAndWaitForEventSignature OnCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBRPlayMontageAndWaitForEventSignature OnBlendOutDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBRPlayMontageAndWaitForEventSignature OnInterrumpedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBRPlayMontageAndWaitForEventSignature OnCanceledDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBRPlayMontageAndWaitForEventSignature OnEventReceivedDelegate;

public:
	UBRT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "Owning Ability", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UBRT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer 
		EventTags, float Rate = 1.0f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.0f);


public:
	virtual void Activate() override;

	/** Called when the task is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bIsInterrupted);

	void OnAbilityCanceled();

	void OnMontageEnded(UAnimMontage* Montage, bool bIsInterrupted);

	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	bool StopPlayingMontage();
};
