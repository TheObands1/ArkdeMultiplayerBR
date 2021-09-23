// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorations/BR_DecorativeElement.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABR_DecorativeElement::ABR_DecorativeElement()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	DestroyOnStartProbability = 50.0f;
}

// Called when the game starts or when spawned
void ABR_DecorativeElement::BeginPlay()
{
	Super::BeginPlay();
	
	int randomInteger = FMath::RandRange(0, 100);

	if (randomInteger <= DestroyOnStartProbability)
	{
		Destroy();
	}
}


