// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageSystemComponent.h"

#include "Utils/DamageSystemTypes.h"


// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UDamageSystemComponent::HandleIncomingDamage(FDamageInfo& DamageInfo)
{
	if (IsDead) { return false;}
	
	if (IsInvincible && !DamageInfo.ShouldDamageInvincible || IsBlocking && DamageInfo.CanBeBlocked)
	{
		return false;
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth-DamageInfo.DamageAmount, 0.0f, MaxHealth);
	 return true;
}

void UDamageSystemComponent::HandleIncomingHeal(float HealAmmount, AActor* Healer)
{
	if (IsDead) {return;}
	CurrentHealth =FMath::Clamp(CurrentHealth + HealAmmount, 0.f, MaxHealth);
}


