// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utils/DamageSystemTypes.h"
#include "DamagableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROYECTOFINAL_API IDamagableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damagable Interface")
	float GetCurrentHealth();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damagable Interface")
	float GetMaxHealth();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damagable Interface")
	bool GetIsDeath();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damagable Interface")
	void Heal(float HealAmount, AActor* Healer);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Damagable Interface")
	bool TakeDamage(FDamageInfo& DamageInfo);
};
