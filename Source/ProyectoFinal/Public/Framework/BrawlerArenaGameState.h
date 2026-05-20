// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BrawlerArenaGameState.generated.h"

class ABrawlerArenaPlayerState;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API ABrawlerArenaGameState : public AGameState
{
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnGameEnded(ABrawlerArenaPlayerState* WinnerPlayerState);
	
	// Clase de widget que asignaremos a WBP_Results
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ResultsWidgetClass;
	
};
