// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Base.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
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
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
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

