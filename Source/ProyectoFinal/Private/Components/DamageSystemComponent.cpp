// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageSystemComponent.h"

#include "Net/UnrealNetwork.h"
#include "Utils/DamageSystemTypes.h"


// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// No se si es necesario que este en el tick
	PrimaryComponentTick.bCanEverTick = true;
	
	// Para todos los que lo tienen
	SetIsReplicated(true);
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	// ...
}

void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDamageSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDamageSystemComponent, CurrentHealth);
	DOREPLIFETIME(UDamageSystemComponent, MaxHealth);
	DOREPLIFETIME(UDamageSystemComponent, bIsDead);
	DOREPLIFETIME(UDamageSystemComponent, bIsBlocking);
	DOREPLIFETIME(UDamageSystemComponent, bIsInvincible);
}

void UDamageSystemComponent::OnRep_HealthChanged() const
{
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Se replico la vida: %f / %f"), CurrentHealth, MaxHealth));
}
	

bool UDamageSystemComponent::HandleIncomingDamage(const FDamageInfo& DamageInfo)
{
	// Solo el servidor maneja el daño
	if (!GetOwner()->HasAuthority())
	{
		Server_HandleIncomingDamage(DamageInfo);
		return true;
	}
	
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

void UDamageSystemComponent::Server_HandleIncomingDamage_Implementation(const FDamageInfo& DamageInfo)
{
	HandleIncomingDamage(DamageInfo);
}

bool UDamageSystemComponent::Server_HandleIncomingDamage_Validate(const FDamageInfo& DamageInfo)
{
	return DamageInfo.DamageAmount>=0.0f;
}

void UDamageSystemComponent::HandleIncomingHeal(float HealAmount, AActor* Healer)
{
	// Solo el servidor maneja la curacion
	if (!GetOwner()->HasAuthority()){return;}
	if (bIsDead) {return;}
	CurrentHealth =FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealRecived.Broadcast(HealAmount, Healer);
}


