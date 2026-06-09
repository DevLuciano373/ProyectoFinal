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
	if (CollisionSword)
	{
		CollisionSword->OnComponentBeginOverlap.AddDynamic(this, &ASwordWeapon::OnOverlapBegin);
	}
}

// Called every frame
void ASwordWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASwordWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	if (HasAuthority())
	{
		DamageInfo.DamageCauser = GetOwner();
		DamageInfo.DamageAmount = 45.0f;
		DamageInfo.CanBeBlocked = true;
		DamageInfo.CanBeParried = true;
		DamageInfo.DamageResponse = EDamageResponse::HitReaction;
		DamageInfo.DamageType = EDamageType::Physical;
		ApplyDamageToActor(OtherActor, DamageInfo);
	}
	else
	{
		ServerRequestDamage(OtherActor, DamageInfo);
	}
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

void ASwordWeapon::SetHitBoxActive(const bool bActive) const
{
	if (!CollisionSword)return;
	
	if (bActive)
	{
		CollisionSword->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSword->SetGenerateOverlapEvents(true);
		CollisionSword->ShapeColor = FColor::Yellow;
		CollisionSword->MarkRenderStateDirty();
		
	}
	else
	{
		CollisionSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionSword->SetGenerateOverlapEvents(false);
		CollisionSword->ShapeColor = FColor::Red;
		CollisionSword->MarkRenderStateDirty();
	}
}
