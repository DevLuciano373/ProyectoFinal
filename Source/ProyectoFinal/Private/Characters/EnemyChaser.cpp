// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyChaser.h"

#include "Components/CapsuleComponent.h"
#include "Framework/BrawlerArenaGameState.h"
#include "Framework/BrawlerArenaPlayerState.h"


// Sets default values
AEnemyChaser::AEnemyChaser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyChaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyChaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyChaser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyChaser::RespondToDeath_Implementation()
{
	Super::RespondToDeath_Implementation();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	if (ABrawlerArenaGameState* GS = GetWorld()->GetGameState<ABrawlerArenaGameState>())
	{
		if (ABrawlerArenaPlayerState* PS = GetPlayerState<ABrawlerArenaPlayerState>())
		{
				GS->OnEnemyKilled();
		}
	}
	Destroy();
}

