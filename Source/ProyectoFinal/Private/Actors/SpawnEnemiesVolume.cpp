// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnEnemiesVolume.h"

#include "Components/BoxComponent.h"


// Sets default values
ASpawnEnemiesVolume::ASpawnEnemiesVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SpawnEnemiesVolume = CreateDefaultSubobject<UBoxComponent>("SpawnEnemiesVolume");
	SpawnEnemiesVolume->SetBoxExtent(FVector(500.0f, 500.0f, 500.0f));
	SpawnEnemiesVolume->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ASpawnEnemiesVolume::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemiesVolume->ShapeColor = FColor::Yellow;
	SpawnEnemiesVolume->MarkRenderStateDirty();
}

// Called every frame
void ASpawnEnemiesVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

