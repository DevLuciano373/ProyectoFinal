// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Utils/WarriorType.h"
#include "BrawlerArenaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API ABrawlerArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	
	
	void SetWarriorType(EWarriorType Type);
	EWarriorType GetWarriorType() const;
	
	UPROPERTY(ReplicatedUsing=OnRep_WarriorType)
	EWarriorType WarriorType = EWarriorType::None;
	
	UFUNCTION()
	void OnRep_WarriorType();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void AddOneKill(float ScoreAmount);

};
