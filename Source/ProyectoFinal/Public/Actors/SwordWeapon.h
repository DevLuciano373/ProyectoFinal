// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/DamageSystemTypes.h"
#include "SwordWeapon.generated.h"

class UBoxComponent;

UCLASS()
class PROYECTOFINAL_API ASwordWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwordWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> CollisionSword;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> SwordMesh;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerRequestDamage(AActor* DamagedActor, FDamageInfo Damage);
	
	void ApplyDamageToActor(const AActor* DamagedActor, const FDamageInfo& Damage);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetHitBoxActive(bool bActive) const;

	UPROPERTY()
	FDamageInfo DamageInfo;
	
};
