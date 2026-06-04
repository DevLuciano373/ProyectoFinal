// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamagableInterface.h"
#include "Character_Base.generated.h"

class UDamageSystemComponent;

UCLASS()
class PROYECTOFINAL_API ACharacter_Base : public ACharacter, public IDamagableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void RespondToDamageTaken(const FDamageInfo& DamageInfo);
	
	UFUNCTION(BlueprintNativeEvent)
	void RespondToDamageAvoided(const FDamageInfo& DamageInfo);
	
	UFUNCTION(BlueprintNativeEvent)
	void RespondToHealRecived(float HealAmount, AActor* Healer);
	
	UFUNCTION(BlueprintNativeEvent)
	void RespondToDeath();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
public:	
	// Damage System
	
	virtual float GetMaxHealth_Implementation() override;
	virtual float GetCurrentHealth_Implementation() override;
	virtual bool GetIsDeath_Implementation() override;
	virtual void Heal_Implementation(float HealAmount, AActor* Healer) override;
	virtual bool TakeDamage_Implementation(const FDamageInfo& DamageInfo) override;

	// Damage Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDamageSystemComponent> DamageSystemComponent;
};
