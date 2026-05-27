// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProyectoFinalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProyectoFinal.h"
#include "Components/BoxComponent.h"
#include "Framework/BrawlerArenaPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/WarriorType.h"

AProyectoFinalCharacter::AProyectoFinalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	// Agregamos la colision para el ataque
	DashAttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DashAttackHitBox"));
	DashAttackHitBox->InitBoxExtent(FVector(32.f, 32.f, 32.f)); // Define el tamaño (ajusta los valores a tu gusto)
	DashAttackHitBox->SetupAttachment(GetMesh(), FName("dash_socket"));
	DashAttackHitBox->SetGenerateOverlapEvents(true);

	DashAttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DashAttackHitBox->SetCollisionObjectType(ECC_WorldDynamic);
	DashAttackHitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	DashAttackHitBox->SetHiddenInGame(false);
}

void AProyectoFinalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProyectoFinalCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AProyectoFinalCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProyectoFinalCharacter::Look);
		
		// Esta accion la agregamos nosotros!!
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AProyectoFinalCharacter::DoAttack);
	}
	else
	{
		UE_LOG(LogProyectoFinal, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProyectoFinalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AProyectoFinalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AProyectoFinalCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AProyectoFinalCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AProyectoFinalCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AProyectoFinalCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AProyectoFinalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ABrawlerArenaPlayerState* PS = Cast<ABrawlerArenaPlayerState>(NewController->PlayerState);
	if (PS)
	{
		EWarriorType WarriorType = PS->GetWarriorType();
		UpdateWarriorMaterial(WarriorType);
		
	}
}

void AProyectoFinalCharacter::UpdateWarriorMaterial(EWarriorType WarriorType)
{
	switch (WarriorType)
	{
	case EWarriorType::None:
		break;
	case EWarriorType::Barbarian:
		GetMesh()->SetColorParameterValueOnMaterials(FName("Paint Tint"), FColor::Orange);
		break;
	case EWarriorType::Archer:
		GetMesh()->SetColorParameterValueOnMaterials(FName("Paint Tint"), FColor::Green);
		break;
	case EWarriorType::Mage:
		GetMesh()->SetColorParameterValueOnMaterials(FName("Paint Tint"), FColor::Blue);
		break;
	}
}

void AProyectoFinalCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	ABrawlerArenaPlayerState* PS = Cast<ABrawlerArenaPlayerState>(GetPlayerState());
	if (PS)
	{
		EWarriorType WarriorType = PS->GetWarriorType();
		UpdateWarriorMaterial(WarriorType);
		
	}
}

void AProyectoFinalCharacter::BeginPlay()
{
	Super::BeginPlay();
	DashAttackHitBox->ShapeColor = FColor::Blue;
	DashAttackHitBox->MarkRenderStateDirty();
	DashAttackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProyectoFinalCharacter::OnAttackOverlap);
	
}

void AProyectoFinalCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	DashAttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DashAttackHitBox->ShapeColor = FColor::Blue;
}

void AProyectoFinalCharacter::DoAttack()
{
		
	bIsAttacking = true;
	OnAttackMontageEnded.BindUObject(this, &AProyectoFinalCharacter::AttackMontageEnded);
	DashAttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DashAttackHitBox->ShapeColor = FColor::Yellow;
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ChargedAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		
		if (MontageLength > 0.0f)
		{
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChargedAttackMontage);
		}
	}

}
void AProyectoFinalCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor !=this)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			20.0f, 
			GetController(), this, UDamageType::StaticClass());
		DashAttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

