// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameState.h"

#include "ProyectoFinalPlayerController.h"
#include "Actors/SpawnEnemiesVolume.h"
#include "Framework/BrawlerArenaGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


// void ABrawlerArenaGameState::Multicast_OnGameEnded_Implementation(ABrawlerArenaPlayerState* WinnerPlayerState)
// {
// 	// Bloquear los controladores del jugador local
// 	APlayerController* PC = GetWorld()->GetFirstPlayerController();
// 	if (PC && PC->IsLocalController())
// 	{
// 		PC->SetInputMode(FInputModeUIOnly());
// 		PC->bShowMouseCursor = true;
// 		
// 		// Mostrar el widget de ganador o perdedor
// 		// OnShowResultsUI(WinnerPlayerState);
// 	}
// 	
// }

void ABrawlerArenaGameState::SetGamePhase(EMatchPhase NewPhase)
{
	if (!HasAuthority()) return;
	if (CurrentPhase != NewPhase)
	{
		CurrentPhase = NewPhase;
		OnPhaseChanged.Broadcast(CurrentPhase);
	}
}

void ABrawlerArenaGameState::SetCountdownTime(int32 NewTime)
{
	if (!HasAuthority())return;
	{
		WaveCountDown = GetRemainingTime();
		OnCountdownChanged.Broadcast(WaveCountDown);
	}
}

void ABrawlerArenaGameState::OnRep_CountdownTime()
{
	// Cuando el cliente recibe el nuevo valor, dispara el delegado para su UI
	OnCountdownChanged.Broadcast(WaveCountDown);
}

float ABrawlerArenaGameState::GetRemainingTime() const
{
	// Si aún no se ha establecido un tiempo, devolvemos 0
	if (WaveCountDown <= 0.0f) return 0.0f;
 
	// Restamos el tiempo actual sincronizado del servidor al objetivo
	float Remaining = WaveCountDown - GetServerWorldTimeSeconds();
	return FMath::Max(Remaining, 0.0f);
}

void ABrawlerArenaGameState::AddSpawnZone(ASpawnEnemiesVolume* Zona)
{
	ZonasSpawn.Add(Zona);
}

void ABrawlerArenaGameState::SpawnEnemiesForWave()
{
	// iteramos de uno hasta el numero de enemigos en la ola
	for (int i = 0; i < EnemiesInThisWave; ++i)
	{
		if (ZonasSpawn.IsEmpty())return;
		int8 RandomIndex = FMath::RandRange(0, ZonasSpawn.Num() - 1);
		ASpawnEnemiesVolume* ZonaElegida = ZonasSpawn[RandomIndex];
		ZonaElegida->SpawnSingleEnemy();		
	}
}

void ABrawlerArenaGameState::OnKilledEnemy()
{
	if (!HasAuthority())return;
	
	ActiveEnemies = FMath::Clamp(ActiveEnemies-1, 0, EnemiesInThisWave);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("ActiveEnemies: %i"), ActiveEnemies));
	if (ActiveEnemies == 0)
	{
		ABrawlerArenaGameMode* GM = GetWorld()->GetAuthGameMode<ABrawlerArenaGameMode>();
		if (GM)
		{
			GM->EndWave();
		}
		
	}
}

void ABrawlerArenaGameState::SetEnemiesInThisWave(float Quantity)
{
	EnemiesInThisWave = Quantity;
}

void ABrawlerArenaGameState::AddEnemyToWave()
{
	ActiveEnemies +=1;
}

void ABrawlerArenaGameState::OnRep_MatchPhase()
{
	OnPhaseChanged.Broadcast(CurrentPhase);
}

void ABrawlerArenaGameState::GetWinnerPlayerState()
{
	if (!HasAuthority()) return;
	
	APlayerState* BestPlayer = nullptr;
	float MaxScore = -1.f;
	
	for (APlayerState* PS : PlayerArray)
	{
		if (PS && PS->GetScore() > MaxScore)
		{
			MaxScore = PS->GetScore();
			BestPlayer = PS;
		}
	}
	
	if (BestPlayer)
	{
		WinnerName = BestPlayer->GetPlayerName();
	}
	
}

void ABrawlerArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentPhase);
	DOREPLIFETIME(ThisClass, WinnerName);
	DOREPLIFETIME(ThisClass, WaveCountDown);
}
