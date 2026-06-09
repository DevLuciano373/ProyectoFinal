// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ANS_SwordCollision.h"

#include "ProyectoFinalCharacter.h"
#include "Actors/SwordWeapon.h"

void UANS_SwordCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AProyectoFinalCharacter* Character = Cast<AProyectoFinalCharacter>(MeshComp->GetOwner()))
	{
		if (Character->EquippedSword)
		{
			Character->EquippedSword->SetHitBoxActive(true);
		}
	}
}

void UANS_SwordCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (AProyectoFinalCharacter* Character = Cast<AProyectoFinalCharacter>(MeshComp->GetOwner()))
	{
		if (Character->EquippedSword)
		{
			Character->EquippedSword->SetHitBoxActive(false);
		}
	}
}
