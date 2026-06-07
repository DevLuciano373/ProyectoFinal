// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnEnemiesVolume.generated.h"

class UBoxComponent;

UCLASS()
class PROYECTOFINAL_API ASpawnEnemiesVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnEnemiesVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawningZone", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> SpawnEnemiesVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawningZone", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawningZone", meta=(AllowPrivateAccess="true"))
	float SpawnRate = 3.0f;
	
	UFUNCTION()
	void SpawnEnemy() const;
	
	// Para que no spawneen abajo del mapa o adentro de una mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawningZone", meta=(AllowPrivateAccess="true"))
	float SpawnOffsetZ = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawningZone", meta=(AllowPrivateAccess="true"))
	float TraceDistance = 2000.0f;
	
	bool GetValidateSpawnPoint(FVector& OutLocation) const;
	
public:
	// Para llamar desde el GS
	UFUNCTION(Category = "SpawningZone")
	void SpawnSingleEnemy() const;
	
};
