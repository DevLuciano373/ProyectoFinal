// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageSystemTypes.generated.h"
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Physical UMETA(DisplayName = "Physical"),
	Magical UMETA(DisplayName = "Magical"),
	Environmental UMETA(DisplayName = "Environmental"),
};

UENUM(BlueprintType)
enum class EDamageResponse: uint8
{
	None UMETA(DisplayName = "None"),
	HitReaction UMETA(DisplayName = "HitReaction"),
	Knockback UMETA(DisplayName = "Knockback"),
	Stagger UMETA(DisplayName = "Stagger"),
	Stun UMETA(DisplayName = "Stun"),
	
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY();
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	AActor* DamageCauser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool CanBeBlocked = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool CanBeParried = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool ShouldDamageInvincible = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool ShouldForceInterrupt = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	EDamageType DamageType = EDamageType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	EDamageResponse DamageResponse = EDamageResponse::None;
};
