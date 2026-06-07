// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SwordWeapon.h"

#include "Components/BoxComponent.h"
#include "Interfaces/DamagableInterface.h"


// Sets default values
ASwordWeapon::ASwordWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("SwordMesh");
	SwordMesh->SetupAttachment(RootComponent);
	RootComponent = SwordMesh;
	
	CollisionSword = CreateDefaultSubobject<UBoxComponent>("CollisionSword");
	CollisionSword->SetBoxExtent(FVector(20.0f, 16.0f, 75.0f));
	CollisionSword->SetHiddenInGame(false);
	CollisionSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSword->SetGenerateOverlapEvents(false); // Optimización: no busca solapamientos si está apagado
	CollisionSword->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();
	CollisionSword->ShapeColor = FColor::Red;
	CollisionSword->MarkRenderStateDirty();
}

// Called every frame
void ASwordWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASwordWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor)
{
	if (!HasAuthority())return;
	
	if (OtherActor && OtherActor != GetOwner())
	{
		if (OtherActor->GetClass()->ImplementsInterface(UDamagableInterface::StaticClass()))
		{
			IDamagableInterface::Execute_TakeDamage(OtherActor, DamageInfo);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Ejecutando desde la interfaz")));
		}
		
	}
}


void ASwordWeapon::SetHitBoxActive(const bool bActive) const
{
	if (!CollisionSword)return;
	
	if (bActive)
	{
		CollisionSword->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSword->SetGenerateOverlapEvents(true);
		CollisionSword->ShapeColor = FColor::Yellow;
		
	}
	else
	{
		CollisionSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionSword->SetGenerateOverlapEvents(false);
		CollisionSword->ShapeColor = FColor::Red;
	}
}

