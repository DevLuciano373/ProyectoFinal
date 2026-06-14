// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "EnemyChaser.generated.h"

UCLASS()
class PROYECTOFINAL_API AEnemyChaser : public ACharacter_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyChaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Llamo a esta funcion para agregarse al array de enemigos en la oleada en el gamemode
	void AddEnemyToWave();
	
	// Puntos por muerte, deberia crear una clase base que los contenga, pero no quiero
	UPROPERTY(BlueprintReadWrite)
	float KillPoints = 10.0;
	
	FDamageInfo DamageRecived;
	
	virtual void RespondToDamageTaken_Implementation(const FDamageInfo& DamageInfo) override;
	virtual void RespondToDeath_Implementation() override;
	
	FTimerHandle WaitDeath;
};
