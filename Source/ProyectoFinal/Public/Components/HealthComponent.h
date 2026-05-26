// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EHealthModifierType: uint8
{
	Damage UMETA(DisplayName="Daño"),
	Healing UMETA(DisplayName="Curacion")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, CurrentHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOFINAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category="Events")
	FOnHealthChange OnHealthChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing=OnRep_CurrentHealth, Category="Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,ReplicatedUsing=OnRep_CurrentHealth, Category="Health")
	float MaxHealth;

	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int DamageTicksReceived;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int HealthTicksReceived;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(int DamageAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(int HealAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void HandleModifierEffect(AActor* AffectedActor, EHealthModifierType EffectType);
};
