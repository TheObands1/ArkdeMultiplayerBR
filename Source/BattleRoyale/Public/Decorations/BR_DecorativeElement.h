// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BR_DecorativeElement.generated.h"

class UStaticMeshComponent;

UCLASS()
class BATTLEROYALE_API ABR_DecorativeElement : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Destroy Probability", meta = (ClampMin = 0.0, UIMin = 0.0))
	int DestroyOnStartProbability;
	
public:	
	// Sets default values for this actor's properties
	ABR_DecorativeElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CheckSwitchHasAuthority();

	UFUNCTION(BlueprintCallable)
	void DestroyOnStart();

};
