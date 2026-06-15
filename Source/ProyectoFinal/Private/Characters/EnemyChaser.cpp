// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyChaser.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/BrawlerArenaGameState.h"
#include "Framework/BrawlerArenaPlayerState.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AEnemyChaser::AEnemyChaser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);

}

// Called when the game starts or when spawned
void AEnemyChaser::BeginPlay()
{
	Super::BeginPlay();
	
	// Cuando se spawnea un enemigo se agrega a la ola
	AddEnemyToWave();
}

void AEnemyChaser::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsAttacking);
}

// Called every frame
void AEnemyChaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AEnemyChaser::AddEnemyToWave()
{
	if (!HasAuthority())return;
	ABrawlerArenaGameState* GS = GetWorld()->GetGameState<ABrawlerArenaGameState>();
	if (GS)
	{
		GS->AddEnemyToWave();
	}
}


void AEnemyChaser::RespondToDamageTaken_Implementation(const FDamageInfo& DamageInfo)
{
	Super::RespondToDamageTaken_Implementation(DamageInfo);
	DamageRecived = DamageInfo;
}

void AEnemyChaser::RespondToDeath_Implementation()
{
	Super::RespondToDeath_Implementation();
	
	// lo que pasa si se muere
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(WaitDeath, 4.0f, AEnemyChaser::Destroy(), false);
	bIsDead = true;

	// A chequear si es asi
	AActor* OtherActor = DamageRecived.DamageCauser;
	if (!OtherActor)return; 
	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (OtherPawn && HasAuthority())
	{
		APlayerController* KillerController = Cast<APlayerController>(OtherPawn->GetController());
	
		if (KillerController)
		{
			ABrawlerArenaPlayerState* PS = KillerController->GetPlayerState<ABrawlerArenaPlayerState>();
			if (PS)
			{
				PS->AddOneKill(KillPoints);
				GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, FString::Printf(TEXT("Puntos otorgados a %s, nuevo score: %f"), *PS->GetName(), KillPoints));
			}
		}
	}


}


void AEnemyChaser::ActivateAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyChaser::DeactivateAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyChaser::PerformAttack()
{
	if (!AttackMontage) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		float Duration = PlayAnimMontage(AttackMontage, 1.0f);
		if (Duration > 0.f)
		{
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
		}
		else
		{
			bIsAttacking = false;
		}
	}
}



void AEnemyChaser::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageFinished.Broadcast(Montage);
}


