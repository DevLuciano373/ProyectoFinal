// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaPlayerState.h"
#include "Utils/WarriorType.h"
#include "Net/UnrealNetwork.h"

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
