// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameMode.h"

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
}
