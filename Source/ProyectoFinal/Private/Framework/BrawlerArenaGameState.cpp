// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameState.h"

#include "Actors/SpawnEnemiesVolume.h"
#include "Framework/BrawlerArenaGameMode.h"


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
