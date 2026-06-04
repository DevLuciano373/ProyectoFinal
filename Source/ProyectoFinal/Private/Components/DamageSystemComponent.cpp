// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageSystemComponent.h"

#include "Net/UnrealNetwork.h"
#include "Utils/DamageSystemTypes.h"


// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDamageSystemComponent::OnRep_MaxHealth() const
{
	// Para la UI
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UDamageSystemComponent::OnRep_CurrentHealth() const
{
	// Para la UI
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UDamageSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDamageSystemComponent, CurrentHealth);
	DOREPLIFETIME(UDamageSystemComponent, MaxHealth);
}


// Called when the game starts
void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UDamageSystemComponent::HandleIncomingDamage(const FDamageInfo& DamageInfo)
{
	if (bIsDead) { return false;}
	
	if (bIsInvincible && !DamageInfo.ShouldDamageInvincible || bIsBlocking && DamageInfo.CanBeBlocked)
	{
		OnDamageAvoided.Broadcast(DamageInfo);
		return false;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageInfo.DamageAmount, 0.0f, MaxHealth);
	
	OnDamageTaken.Broadcast(DamageInfo);
	
	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
	
	return true;
}

void UDamageSystemComponent::HandleIncomingHeal(float HealAmount, AActor* Healer)
{
	if (bIsDead) {return;}
	CurrentHealth =FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealRecived.Broadcast(HealAmount, Healer);
	
}


