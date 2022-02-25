// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BR_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEROYALE_API ABR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conclusion")
	TSubclassOf<AActor> ViewActorClass;

protected: 
	virtual void OnPossess(APawn* aPawn) override;

public:
	void GameConclusion(bool bWasConclusionSuccesful);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GameConclusion(bool bWasConclusionSuccesful);

	
};
