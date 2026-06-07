// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHud.h"

#include "ProyectoFinalCharacter.h"
#include "Components/DamageSystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) {return;}
	APawn* Pawn = PlayerController->GetPawn();
	if (!Pawn) {return;}
	DamageComponent = Pawn->FindComponentByClass<UDamageSystemComponent>();
	if (DamageComponent)
	{
		DamageComponent->OnHealthChanged.AddDynamic(this, &UPlayerHud::UpdateHealth);
		UpdateHealth(DamageComponent->CurrentHealth, DamageComponent->MaxHealth);
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
	GEngine->AddOnScreenDebugMessage(-1, 0.2f,FColor::Red,FString::Printf(TEXT("%f / %f"), CurrentHealth, MaxHealth));
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

