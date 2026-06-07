// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BrawlerArenaGameState.generated.h"

class ASpawnEnemiesVolume;
class UWaveCountdownWidget;
class ABrawlerArenaPlayerState;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API ABrawlerArenaGameState : public AGameState
{
	GENERATED_BODY()
public:
	// Devuelve el jugador que gano como un player state?
	APlayerController GetWinnerPlayerState() const;
	
	UPROPERTY()
	TArray<ASpawnEnemiesVolume*> ZonasSpawn;
	
	UFUNCTION()
	void AddSpawnZone(ASpawnEnemiesVolume* Zona);
	
	UFUNCTION()
	void SpawnEnemiesForWave();
	
	// Cantidad Maxima de enemigos en la ola actual
	UPROPERTY()
	int EnemiesInThisWave;
	
	// Enemigos actuales en la ola
	
	UPROPERTY()
	int ActiveEnemies;
	
	
	UFUNCTION()
	void OnKilledEnemy();
	
	UFUNCTION()
	void SetEnemiesInThisWave(float Quantity);
	
	// Array con todos los enemigos en la ola actual
	UFUNCTION()
	void AddEnemyToWave();
	
};
