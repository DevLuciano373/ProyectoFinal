// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaPlayerState.h"

#include "Framework/BrawlerArenaGameState.h"
#include "Utils/WarriorType.h"
#include "Net/UnrealNetwork.h"

// Para actualizar los puntos en el UI
void ABrawlerArenaPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	OnScoreChanged.Broadcast(GetScore());
}

void ABrawlerArenaPlayerState::SetWarriorType(EWarriorType Type)
{
	WarriorType = Type;
}

EWarriorType ABrawlerArenaPlayerState::GetWarriorType() const
{
	return WarriorType;
}

void ABrawlerArenaPlayerState::OnRep_WarriorType()
{
}

void ABrawlerArenaPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, WarriorType);
}

void ABrawlerArenaPlayerState::AddOneKill(const float ScoreAmount)
{
	if (!HasAuthority())return;
	const float NewScore = GetScore()+ScoreAmount;
	SetScore(NewScore);
	OnScoreChanged.Broadcast(NewScore);
	ABrawlerArenaGameState* GS = GetWorld()->GetGameState<ABrawlerArenaGameState>();
	if (GS)
	{
		GS->OnKilledEnemy();
	}
}


