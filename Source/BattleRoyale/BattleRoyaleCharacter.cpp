// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleRoyaleCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySystem/BR_AttributeSet.h"
#include "GameplayAbilitySystem/BR_GameplayAbility.h"
#include "GameplayAbilitySystem/BR_GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "BattleRoyale/BattleRoyale.h"
#include "Components/CapsuleComponent.h"
#include "Core/BR_PlayerState.h"
#include "Animation/AnimMontage.h"
#include "Core/BR_GameModeBase.h"


//////////////////////////////////////////////////////////////////////////
// ABattleRoyaleCharacter

ABattleRoyaleCharacter::ABattleRoyaleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MeleeRightFootSocketName = "AbilityRightFootSocket";

	MeleeRightFootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeLeftFootComponent"));
	MeleeRightFootComponent->SetupAttachment(GetMesh(), MeleeRightFootSocketName);
	MeleeRightFootComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeRightFootComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	MeleeRightFootComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Gameplay Ability System 
	//AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AttributeSet = CreateDefaultSubobject<UBR_AttributeSet>(TEXT("AttributeSet"));
    //AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	bIsInputBound = false;
	bHaveAbilitiesBeenGiven = false;
	bHaveEffectsBeenGiven = false;
	bIsCharacterDead = false;

}

//////////////////////////////////////////////////////////////////////////
// Input

void ABattleRoyaleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABattleRoyaleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABattleRoyaleCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABattleRoyaleCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABattleRoyaleCharacter::LookUpAtRate);

	// handle touch devices
	/*
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABattleRoyaleCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABattleRoyaleCharacter::TouchStopped);

	VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABattleRoyaleCharacter::OnResetVR);
	*/

	SetupGASInputs();
}

void ABattleRoyaleCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeleeRightFootComponent->OnComponentBeginOverlap.AddDynamic(this, &ABattleRoyaleCharacter::OnLightingSlashAbilityOverlap);
	//BP_ApplyGameplayEffectToSelf();

}

void ABattleRoyaleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABR_PlayerState* MyPlayerState = GetPlayerState<ABR_PlayerState>();

	if (IsValid(MyPlayerState))
	{
		AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
		MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, this);
		AttributeSet = MyPlayerState->GetAttributeSet();
		AttributeSet->OnHealthChangedDelegate.AddDynamic(this, &ABattleRoyaleCharacter::OnCharacterHealthResourceChanged);
		AttributeSet->OnManaChangedDelegate.AddDynamic(this, &ABattleRoyaleCharacter::OnCharacterManaResourceChanged);
		SetupAbilities();
		SetupEffects();
	}
}

UAbilitySystemComponent* ABattleRoyaleCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABattleRoyaleCharacter::SetupGASInputs()
{
	if (!bIsInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		//Gameplay Ability System
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds("Confirm", "Cancel", "EBR_AbilityInputID", static_cast<int32>(EBR_AbilityInputID::Confirm), static_cast<int32>(EBR_AbilityInputID::Cancel)));
		bIsInputBound = true;
	}
}

void ABattleRoyaleCharacter::SetupAbilities()
{
	if (!(GetLocalRole() == ENetRole::ROLE_Authority) || !IsValid(AbilitySystemComponent) || bHaveAbilitiesBeenGiven)
	{
		return;
	}

	for (TSubclassOf<UBR_GameplayAbility>& CurrentAbility : StartingAbilities)
	{
		if (IsValid(CurrentAbility))
		{
			UBR_GameplayAbility* DefaultObject = CurrentAbility->GetDefaultObject<UBR_GameplayAbility>();
			FGameplayAbilitySpec AbilitySpec(DefaultObject, 1, static_cast<int32>(DefaultObject->AbilityInputID), this);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	bHaveAbilitiesBeenGiven = true;
}

void ABattleRoyaleCharacter::SetupEffects()
{
	if (!(GetLocalRole() == ENetRole::ROLE_Authority) || !IsValid(AbilitySystemComponent) || bHaveEffectsBeenGiven)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (TSubclassOf<UBR_GameplayEffect>& CurrentEffect : StartingEffects)
	{
		if (IsValid(CurrentEffect))
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentEffect, 1.f, EffectContextHandle);
			
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), GetAbilitySystemComponent());
			}
		}
	}

	bHaveEffectsBeenGiven = true;
}

void ABattleRoyaleCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABR_PlayerState* MyPlayerState = GetPlayerState<ABR_PlayerState>();

	if (IsValid(MyPlayerState))
	{
		AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
		MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, this);
		AttributeSet = MyPlayerState->GetAttributeSet();
		AttributeSet->OnHealthChangedDelegate.AddDynamic(this, &ABattleRoyaleCharacter::OnCharacterHealthResourceChanged);
		AttributeSet->OnManaChangedDelegate.AddDynamic(this, &ABattleRoyaleCharacter::OnCharacterManaResourceChanged);
		SetupGASInputs();
	}
}


void ABattleRoyaleCharacter::OnResetVR()
{
	// If BattleRoyale is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in BattleRoyale.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABattleRoyaleCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ABattleRoyaleCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ABattleRoyaleCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABattleRoyaleCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABattleRoyaleCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABattleRoyaleCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// ================================ Custom Functions =========================================

void ABattleRoyaleCharacter::OnLightingSlashAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != this)
	{
		ABattleRoyaleCharacter* PossibleBattleRoyaleCharacter = Cast<ABattleRoyaleCharacter>(OtherActor);
		if (IsValid(PossibleBattleRoyaleCharacter) && OtherActor != this)
		{
			BP_OnLightingSlashAbilityOverlap(PossibleBattleRoyaleCharacter);
			BP_ApplyLightingSlashDamage(PossibleBattleRoyaleCharacter);
		}
	}
}

void ABattleRoyaleCharacter::SetMeleeRightFootComponentCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeRightFootComponent->SetCollisionEnabled(NewCollisionState);
}

void ABattleRoyaleCharacter::Server_Die_Implementation(ABattleRoyaleCharacter* KillerCharacter)
{
	if (IsValid(KillerCharacter)) 
	{
		if (bIsCharacterDead)
		{
			return;
		}

		bIsCharacterDead = true;

		if (IsValid(DeathEffectClass))
		{
			FGameplayEffectContextHandle EffectContext;
			AbilitySystemComponent->ApplyGameplayEffectToSelf(DeathEffectClass->GetDefaultObject<UGameplayEffect>(), 1.0f, EffectContext);

		}

		ABR_PlayerState* KillerPlayerState = Cast<ABR_PlayerState>(KillerCharacter->GetPlayerState());

		if (IsValid(KillerPlayerState))
		{
			KillerPlayerState->ScoreKill();
		}
	}


	ABR_GameModeBase* WorldGameMode = Cast<ABR_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(WorldGameMode))
	{
		WorldGameMode->PlayerIsKilled(GetController());
	}

	Multicast_OnDeath();
}

void ABattleRoyaleCharacter::OnCharacterHealthResourceChanged(float CurrentHealth, float MaxHealth)
{
	Client_OnHealthChanged(CurrentHealth, MaxHealth);
}

void ABattleRoyaleCharacter::Client_OnHealthChanged_Implementation(float CurrentHealth, float MaxHealth)
{
	OnCharacterHealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ABattleRoyaleCharacter::OnCharacterManaResourceChanged(float CurrentMana, float MaxMana)
{
	Client_OnManaChanged(CurrentMana, MaxMana);
}

void ABattleRoyaleCharacter::Multicast_OnDeath_Implementation()
{
	if (IsValid(DeathMontage1) && IsValid(DeathMontage2))
	{
		float ProbabilityOfPlayingMontage1 = FMath::FRandRange(0.0f, 1.0f);
		
		if (ProbabilityOfPlayingMontage1 > 0.5f)
		{
			PlayAnimMontage(DeathMontage1);
		}
		else
		{
			PlayAnimMontage(DeathMontage2);
		}
	}
	else
	{
		Destroy();
	}
}

void ABattleRoyaleCharacter::Client_OnManaChanged_Implementation(float CurrentMana, float MaxMana)
{
	OnCharacterManaChangedDelegate.Broadcast(CurrentMana, MaxMana);
}


