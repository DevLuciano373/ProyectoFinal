// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Utils/WarriorType.h"
#include "BrawlerArenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API ABrawlerArenaGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABrawlerArenaGameMode();
	
	virtual void StartMatch() override;
	
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// Aca deberia definir como es que se gana y que hacer al respecto
	// virtual void CheckWinCondition();
	
	// virtual void DeclareWinner();
	
	FTimerHandle MatchTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float MatchStartTimerDuration = 5;
	
	// Cuanto dura la partida?
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float MatchTotalTimerDuration;
private:
	
	UFUNCTION(BlueprintCallable, Category="Warrior Class")
	void AssignWarriorType(APlayerController* NewPlayer);
	
	void RefillWarriorClassesPool();
	
	UPROPERTY()
	TArray<EWarriorType> AvailableClassesPool;
};
