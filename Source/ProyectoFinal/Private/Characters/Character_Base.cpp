// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Base.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DamageSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACharacter_Base::ACharacter_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->MaxWalkSpeed = 600.f;
		MoveComp->JumpZVelocity = 500.f;
		MoveComp->AirControl = 0.35f;
		MoveComp->bOrientRotationToMovement = true; // El personaje rota hacia donde camina
	}
 
	// Permitir que el personaje rote con el controlador (o no)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystemComponent"));
	DamageSystemComponent->SetIsReplicated(true);
	
	
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	if (DamageSystemComponent)
	{
		DamageSystemComponent->OnDamageTaken.AddDynamic(this, &ACharacter_Base::RespondToDamageTaken);
		DamageSystemComponent->OnHealRecived.AddDynamic(this, &ACharacter_Base::RespondToHealRecived);
		DamageSystemComponent->OnDamageAvoided.AddDynamic(this, &ACharacter_Base::RespondToDamageAvoided);
		DamageSystemComponent->OnDeath.AddDynamic(this, &ACharacter_Base::RespondToDeath);
		
	}
	
	
}

void ACharacter_Base::RespondToDamageAvoided_Implementation(const FDamageInfo& DamageInfo)
{
}

void ACharacter_Base::RespondToDamageTaken_Implementation(const FDamageInfo& DamageInfo)
{
}


void ACharacter_Base::RespondToHealRecived_Implementation(float HealAmount, AActor* Healer)
{
}

void ACharacter_Base::RespondToDeath_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACharacter_Base::GetMaxHealth_Implementation()
{
	if (!DamageSystemComponent) return 0.0f;
	return DamageSystemComponent->GetMaxHealth();
	
}

float ACharacter_Base::GetCurrentHealth_Implementation()
{
	if (!DamageSystemComponent) return false;
	return DamageSystemComponent->GetCurrentHealth();
}

bool ACharacter_Base::GetIsDeath_Implementation()
{
	if (!DamageSystemComponent) return false;
	return DamageSystemComponent->GetIsDead();
}

void ACharacter_Base::Heal_Implementation(float HealAmount, AActor* Healer)
{
	if (DamageSystemComponent)
	{
		DamageSystemComponent->HandleIncomingHeal(HealAmount, Healer);
	}
}

bool ACharacter_Base::TakeDamage_Implementation(const FDamageInfo& DamageInfo)
{
	if (!DamageSystemComponent) return false;
	
	return DamageSystemComponent->HandleIncomingDamage(DamageInfo);
}

