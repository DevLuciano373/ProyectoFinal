// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameMode.h"
#include "Utils/WarriorType.h"
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

void ABrawlerArenaGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
	// Tomado de la IA de unreal, deberia dispararse cuando hay un error de conexion por buenas practicas
	if (!ErrorMessage.IsEmpty()) return;
	
	// Verificamos que la sala no este llena
	if (GetNumPlayers() > MaxPlayers)
	{
		ErrorMessage = TEXT("Server is full. Elimination of connection attempt.");
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Se alcanzo el numero maximo de jugadores")));
	}
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

// Se dispara cuando el array que define a los guerreros se vació
void ABrawlerArenaGameMode::RefillWarriorClassesPool()
{
	AvailableClassesPool.Empty();
	AvailableClassesPool.Add(EWarriorType::Barbarian);
	AvailableClassesPool.Add(EWarriorType::Mage);
	AvailableClassesPool.Add(EWarriorType::Archer);
	
}

// Determina el ganador
void ABrawlerArenaGameMode::DetermineWinner()
{
	AGameState* GS = GetGameState<ABrawlerArenaGameState>();
	if (!GS) return;
	
	APlayerState* BestPlayer = nullptr;
	float HighestScore = -1.f;
	
	// Iteramos sobre todos los PlayerStateConectados
	for (APlayerState* PS : GS->PlayerArray)
	{
		// Unreal trae un sistema de Score integrado para poder usar rapidamente, lo podemos overraidear
		if (PS && PS-> GetScore() > HighestScore)
		{
			HighestScore = PS->GetScore();
			BestPlayer = PS;
		}
	}
	
	if (BestPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("El ganador es %s"), *BestPlayer->GetName()));
		// Crear un widget que muestre la condicion de victoria?
		// GS->Multicast_OnGameEnded(BestPlayer); para que muestre el widget 
		EndMatch();
	}
}


