// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BrawlerArenaGameState.generated.h"

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
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_OnGameEnded(ABrawlerArenaPlayerState* WinnerPlayerState);
	
	// Clase de widget que asignaremos a WBP_Results
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ResultsWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Wave System", Replicated)
	int CurrentWave = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="GameState", Replicated)
	int ActiveEnemies = 0;
	
	void OnEnemyKilled();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWaveCountdownWidget> CountdownWidgetClass;
	
	void StartIntermission(float Duration);
	void UpdateIntermission();
	
private:
	FTimerHandle IntermissionTimerHandle;
	int SecondsUntilNextWave;
	
	UPROPERTY()
	TObjectPtr<UWaveCountdownWidget> CountdownWidget;
	
	
};
