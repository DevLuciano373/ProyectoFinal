// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHud.h"

#include "ProyectoFinalCharacter.h"
#include "Components/DamageSystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CharacterOwner = Cast<AProyectoFinalCharacter>(GetOwningPlayer()->GetCharacter());
	DamageComponent = Cast<UDamageSystemComponent>(CharacterOwner->FindComponentByClass<UDamageSystemComponent>());
	if (DamageComponent && CharacterOwner)
	{
		DamageComponent->OnHealthChanged.AddDynamic(this, &UPlayerHud::UpdateHealth);	
	}
	
}

void UPlayerHud::NativeDestruct()
{
	if (DamageComponent)
	{
		DamageComponent->OnHealthChanged.RemoveDynamic(this, &UPlayerHud::UpdateHealth);
	}
	Super::NativeDestruct();
}


void UPlayerHud::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
	if (HealthText)
	{
		const FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

