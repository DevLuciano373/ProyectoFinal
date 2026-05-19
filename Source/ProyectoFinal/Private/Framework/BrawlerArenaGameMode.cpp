// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameMode.h"

#include "Framework/BrawlerArenaGameState.h"
#include "Public/Framework/BrawlerArenaPlayerState.h"

ABrawlerArenaGameMode::ABrawlerArenaGameMode()
{
	bDelayedStart = true;
}

void ABrawlerArenaGameMode::StartMatch()
{
	Super::StartMatch();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Comenzo la partida")));
}

void ABrawlerArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MatchTimer, this, &ABrawlerArenaGameMode::StartMatch, MatchStartTimerDuration, false);
}

void ABrawlerArenaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// Definimos una "Clase de jugador" para cada player participando
	AssignWarriorType(NewPlayer);
}

void ABrawlerArenaGameMode::AssignWarriorType(APlayerController* NewPlayer)
{
	// Validamos el PS
	ABrawlerArenaPlayerState* PS = Cast<ABrawlerArenaPlayerState>(NewPlayer->PlayerState);
	if (!PS) return;
	
	// Rellenamos el array
	if (AvailableClassesPool.Num() == 0 )
	{
		RefillWarriorClassesPool();
	}
	
	// Buscamos una clase aleatoria
	int8 RandomIndex = FMath::RandRange(0, AvailableClassesPool.Num() - 1);
	
	// Guardamos el tipo de guerrero elegido
	EWarriorType ChosenType = AvailableClassesPool[RandomIndex];
	
	// Eliminamos la clase del array de disponibles, me enetran dudas sobre cual usar, si RemoveAt o RemoveAtSwap
	AvailableClassesPool.RemoveAtSwap(RandomIndex);

	// Si existe el PS lo asignamos
	if (PS)
	{
		PS->SetWarriorType(ChosenType);
	}
}

void ABrawlerArenaGameMode::RefillWarriorClassesPool()
{
	AvailableClassesPool.Empty();
	AvailableClassesPool.Add(EWarriorType::Barbarian);
	AvailableClassesPool.Add(EWarriorType::Mage);
	AvailableClassesPool.Add(EWarriorType::Archer);
	
}

