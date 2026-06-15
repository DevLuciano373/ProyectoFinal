// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SwordWeapon.h"

#include "ProyectoFinalCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/DamageSystemComponent.h"
#include "Interfaces/DamagableInterface.h"


// Sets default values
ASwordWeapon::ASwordWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	bReplicates=true;
	
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("SwordMesh");
	SwordMesh->SetupAttachment(RootComponent);
	RootComponent = SwordMesh;
	
	
}

// Called when the game starts or when spawned
void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASwordWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASwordWeapon::ServerRequestDamage_Implementation(AActor* DamagedActor, FDamageInfo Damage)
{
	if (!HasAuthority()) return;
	
	ApplyDamageToActor(DamagedActor, Damage);
}

bool ASwordWeapon::ServerRequestDamage_Validate(AActor* DamagedActor, FDamageInfo Damage)
{
	return DamageInfo.DamageAmount >=0.0f && DamageInfo.DamageAmount <= 100.0f;
}

void ASwordWeapon::ApplyDamageToActor(const AActor* DamagedActor, const FDamageInfo& Damage)
{
	UDamageSystemComponent* DSC = DamagedActor->FindComponentByClass<UDamageSystemComponent>();
	if (DSC)
	{
		DSC->HandleIncomingDamage(Damage);
	}
}

