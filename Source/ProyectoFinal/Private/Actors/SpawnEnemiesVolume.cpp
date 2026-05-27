// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnEnemiesVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawnEnemiesVolume::ASpawnEnemiesVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SpawnEnemiesVolume = CreateDefaultSubobject<UBoxComponent>("SpawnEnemiesVolume");
	SpawnEnemiesVolume->SetBoxExtent(FVector(500.0f, 500.0f, 500.0f));
	SpawnEnemiesVolume->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ASpawnEnemiesVolume::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemiesVolume->ShapeColor = FColor::Yellow;
	SpawnEnemiesVolume->MarkRenderStateDirty();
	
	// Timer para spawnear
	if (EnemyClass)
	{
		GetWorldTimerManager().SetTimer(SpawnerTimerHandle, this, &ASpawnEnemiesVolume::SpawnEnemy, SpawnRate, true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("No se definió una clase de enemigo para el Spawn"));
	}
	
}

// Called every frame
void ASpawnEnemiesVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnEnemiesVolume::SpawnEnemy()
{
	UWorld* World = GetWorld();
	
	if (!World && !EnemyClass) return;

	FVector FinalSpawnLocation;
	
	if (GetValidteSpawnPoint(FinalSpawnLocation))
	{
		FRotator Rotation = FRotator(0.0f, UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f), 0.0f);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		World->SpawnActor<AActor>(EnemyClass, FinalSpawnLocation, Rotation, SpawnParams);
	}
}

bool ASpawnEnemiesVolume::GetValidteSpawnPoint(FVector& OutLocation)
{
	UWorld* World = GetWorld();
	if (!World) return false;
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawnEnemiesVolume->GetComponentLocation(), SpawnEnemiesVolume->GetScaledBoxExtent());
	
	FVector TraceStart = RandomPoint;
	FVector TraceEnd = RandomPoint - FVector(0,0, TraceDistance);
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		CollisionParams);
	
	DrawDebugLine(World, TraceStart, TraceEnd, FColor::Green, false, 1.5f);

	if (bHit && HitResult.bBlockingHit)
	{
		OutLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, SpawnOffsetZ);
		return true;
	}
	return false;
	
}






