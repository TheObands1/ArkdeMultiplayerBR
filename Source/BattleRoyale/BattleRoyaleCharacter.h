// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h" 
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BattleRoyaleCharacter.generated.h"

class UBR_AttributeSet;
class UBR_AbilitySystemInterface;
class UBR_GameplayAbility;
class UAbilitySystemComponent;
class UCapsuleComponent;

UCLASS(config = Game)
class ABattleRoyaleCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeRightFootComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Abilities")
	FName MeleeRightFootSocketName;

public:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:
	ABattleRoyaleCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

// -- Custom Variables and functions //

protected:
	UFUNCTION()
	void OnLightingSlashAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnLightingSlashAbilityOverlap(AActor* OverlappedActor);

public:
	void SetMeleeRightFootComponentCollision(ECollisionEnabled::Type NewCollisionState);


// -- Gameplay Ability System elements --

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UAbilitySystemComponent* AbilitySystemComponent;

public:
	//References/Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UBR_AttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
	TArray<TSubclassOf<UBR_GameplayAbility>> StartingAbilities;

public: 
	//Functions
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};

