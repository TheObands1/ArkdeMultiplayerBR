// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ABR_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	SetViewTarget(aPawn);
}

void ABR_PlayerController::GameConclusion(bool bWasConclusionSuccesful)
{
	BP_GameConclusion(bWasConclusionSuccesful);

	TArray<AActor*> ViewPointActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ViewActorClass, ViewPointActors);

	SetViewTargetWithBlend(ViewPointActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
}
