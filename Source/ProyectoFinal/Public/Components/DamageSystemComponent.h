// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystemComponent.generated.h"

struct FDamageInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, const FDamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageAvoided, const FDamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealRecived, float, HealAmount, AActor*, Healer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOFINAL_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageSystemComponent();
	
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, EditAnywhere, BlueprintReadOnly)
	float CurrentHealth = MaxHealth;
	

	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="States")
	bool bIsDead = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="States")
	bool bIsBlocking= false;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="States")
	bool bIsInvincible= false;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_HealthChanged() const;
public:
	
	UFUNCTION(BlueprintCallable, Category="Damage")
	bool HandleIncomingDamage(const FDamageInfo& DamageInfo);

	UFUNCTION(Server, Reliable)
	void Server_HandleIncomingDamage(const FDamageInfo& DamageInfo);
	
	UFUNCTION(BlueprintCallable, Category="Damage")
	void HandleIncomingHeal(float HealAmount, AActor* Healer);
	
	// Get
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Health")
	float GetCurrentHealth(){return CurrentHealth;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Health")
	float GetMaxHealth(){return MaxHealth;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	bool GetIsDead(){return bIsDead;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	bool GetIsBlocking(){return bIsBlocking;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="States")
	bool GetIsInvincible(){return bIsInvincible;}
	
	// Set
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetIsInvincible(bool NewInvincible){bIsInvincible = NewInvincible;}
	
	UFUNCTION(BlueprintCallable, Category="States")
	void SetIsBlocking(const bool NewBlocking) {bIsBlocking = NewBlocking;}
	
	// Delegados
	UPROPERTY(BlueprintAssignable, Category="Damage Events")
	FOnDamageTaken OnDamageTaken;
	
	UPROPERTY(BlueprintAssignable, Category="Damage Events")
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category="Damage Events")
	FOnHealRecived OnHealRecived;
	
	UPROPERTY(BlueprintAssignable, Category="Damage Events")
	FOnDamageAvoided OnDamageAvoided;
	
	UPROPERTY(BlueprintAssignable, Category="Damage Events")
	FOnHealthChanged OnHealthChanged;
};
