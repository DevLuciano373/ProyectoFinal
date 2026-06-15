// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyAttackCollision.h"

#include "Components/DamageSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UEnemyAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	HitActors.Empty();
}

void UEnemyAttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	FVector Start = MeshComp->GetSocketLocation(TEXT("Enemy_Weapon_Start"));
	FVector End = MeshComp->GetSocketLocation(TEXT("Enemy_Weapon_End"));
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());
		
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		MeshComp->GetWorld(),
		Start, 
		End, 
		20.0f, 
		ObjectTypes, 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::ForDuration, 
		HitResult,
		true,
		FColor::Red, 
		FColor::Green, 
		0.5f
		);
	
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		
		if (!HitActors.Contains(HitActor))
		{
			HitActors.AddUnique(HitActor);
			
			UDamageSystemComponent* DamageSystemComponent = HitActor->FindComponentByClass<UDamageSystemComponent>();
			if (DamageSystemComponent)
			{
				AppliedDamage.DamageCauser = MeshComp->GetOwner();
				DamageSystemComponent->Server_HandleIncomingDamage(AppliedDamage);
			}
		}
		
	}
	
}

void UEnemyAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	HitActors.Empty();
}


FString UEnemyAttackCollision::GetNotifyName_Implementation() const
{
	return TEXT("Ventana de colision de daño de enemigo");
}
