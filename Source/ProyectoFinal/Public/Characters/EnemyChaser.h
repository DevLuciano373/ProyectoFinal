// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "EnemyChaser.generated.h"

class UBoxComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontageFinished, UAnimMontage*, Montage);

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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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
	
	UPROPERTY(Replicated)
	bool bIsAttacking = false;
	
	UPROPERTY()
	bool bCanAttack = true;
	
	UPROPERTY()
	bool bIsDead = false;

	// El daño del enemigo, lo hago desde BP
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	FDamageInfo EnemyDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ActivateAttackCollision();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeactivateAttackCollision();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttack();
	
	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnMontageFinished OnMontageFinished;
	
	UPROPERTY(Category="Combat", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> AttackCollision;
private:

	
	UPROPERTY()
	TArray<AActor*> HitActors;
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
