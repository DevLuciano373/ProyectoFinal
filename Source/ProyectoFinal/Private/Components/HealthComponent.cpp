// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...

	TArray<AActor*> FoundAffectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundAffectedActors);

	for (AActor* FoundActor : FoundAffectedActors)
	{
		// Aca tengo que ver si hay una zona de curacion o una zona de daño agregar el if que apunte al Handle, brodcastear OnEffectTick
		// if (ADamageZone* DamageZone = Cast<ADamageZone>(Actor))
		// {
		// 	DamageZone->OnEffectTickApplied.AddDynamic(this, &UHealthComponent::HandleModifierEffect);
		// }
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentHealth);
	DOREPLIFETIME(ThisClass, MaxHealth);
}

void UHealthComponent::TakeDamage(int DamageAmount)
{
	if (DamageAmount >= 0 && CurrentHealth >= 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth- DamageAmount, 0, MaxHealth);
		OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
	}
}

void UHealthComponent::Heal(int HealAmount)
{
	if (HealAmount <= 0 || CurrentHealth <= 0) return;
	CurrentHealth = FMath::Clamp(CurrentHealth+ HealAmount, 0, MaxHealth);
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::HandleModifierEffect(AActor* AffectedActor, EHealthModifierType EffectType)
{
	if (AffectedActor == GetOwner())
	{
		if (EffectType == EHealthModifierType::Damage)
		{
			DamageTicksReceived++;
		}
		if (EffectType == EHealthModifierType::Healing)
		{
			HealthTicksReceived++;
		}
	}
}



