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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownChanged, int32, NewTime);
UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	WaitingToStart UMETA(DisplayName="Starting Game"), // Al inicio del juego, cuenta regresiva inicial
	WaveInProgress UMETA(DisplayName="Wave in progress"), // Jugadores luchando, muestra el hud en el player
	WaitingForNextWave UMETA(DisplayName="Waiting new wave"), // Cuenta regresiva entre oleadas
	GameOver UMETA(DisplayName="The end"), // Mostrar al ganador
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EMatchPhase, NewPhase);

UCLASS()
class PROYECTOFINAL_API ABrawlerArenaGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGamePhaseChanged OnPhaseChanged;
	
	// Función para que el GameMode cambie la fase
	void SetGamePhase(EMatchPhase NewPhase);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCountdownChanged OnCountdownChanged;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Game State")
	float WaveCountDown = 0.0f;
	
	UFUNCTION()
	void SetCountdownTime(int32 NewTime);
	
	UFUNCTION()
	void OnRep_CountdownTime();

	UFUNCTION(BlueprintCallable, Category = "Countdown")
	float GetRemainingTime() const;

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
	
	UPROPERTY(ReplicatedUsing=OnRep_MatchPhase, BlueprintReadOnly, Category="Game State")
	EMatchPhase CurrentPhase;
	
	UFUNCTION()
	void OnRep_MatchPhase();
	
	
	// Devuelve el jugador que gano como un player state?
	void GetWinnerPlayerState();
	
	UFUNCTION()
	FString GetWinnerName()
	{
		return WinnerName;
	}
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Game State")
	FString WinnerName;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
