// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Utils/DamageSystemTypes.h"
#include "EnemyAttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API UEnemyAttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
	public:
	
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageInfo AppliedDamage;
	
	UPROPERTY()
	TArray<AActor*> HitActors;
	
	virtual FString GetNotifyName_Implementation() const override;
};
