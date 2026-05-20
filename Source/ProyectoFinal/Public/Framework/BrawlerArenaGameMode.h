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
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// Aca deberia definir como es que se gana y que hacer al respecto
	// virtual void CheckWinCondition();
	
	// virtual void DeclareWinner();
	
	FTimerHandle MatchTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Match Config")
	float MatchStartTimerDuration = 5;
	
	// Cuantas olas son las que se van a desarrollar en la partida
	UPROPERTY(EditDefaultsOnly, Category="Match Config")
	int32 EnemyWaves = 3; 	
	
	// Numero de olas restantes
	UPROPERTY()
	int32 RemainingWaves;
	
	// Número maximo de jugadores en el match
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Match Config")
	int32 MaxPlayers;
	
private:
	
	UFUNCTION(BlueprintCallable, Category="Warrior Class")
	void AssignWarriorType(APlayerController* NewPlayer);
	
	UFUNCTION()
	void RefillWarriorClassesPool();
	
	// Esta funcion se trigueara cuando RemainingWaves llegue a cero, es decir cuando no queden mas waves
	UFUNCTION()
	void DetermineWinner();

	
	UPROPERTY()
	TArray<EWarriorType> AvailableClassesPool;
	

};
