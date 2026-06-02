// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOFINAL_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageSystemComponent();
	
	UPROPERTY()
	float MaxHealth=100.0f;
	
private:
	
	UPROPERTY()
	float CurrentHealth = MaxHealth;
	

	UPROPERTY()
	bool IsDead = false;
	
	UPROPERTY()
	bool IsBlocking= false;
	
	UPROPERTY()
	bool IsInvincible= false;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category="Damage")
	bool HandleIncomingDamage(FDamageInfo& DamageInfo);

	UFUNCTION(BlueprintCallable, Category="Damage")
	void HandleIncomingHeal(float HealAmmount, AActor* Healer);
	
	// Get
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Health")
	float GetCurrentHealth(){return CurrentHealth;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Health")
	float GetMaxHealth(){return MaxHealth;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	float GetIsDead(){return IsDead;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	float GetIsBlocking(){return IsBlocking;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	float GetIsInvincible(){return IsInvincible;}
	
	// Set
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetIsInvincible(bool NewInvincible){IsInvincible = NewInvincible;}
	
	UFUNCTION(BlueprintCallable, Category="States")
	void SetIsBlocking(const bool NewBlocking) {IsBlocking = NewBlocking;}
};
