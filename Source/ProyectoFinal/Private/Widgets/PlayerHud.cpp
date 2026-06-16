// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHud.h"

#include "ProyectoFinalCharacter.h"
#include "Components/DamageSystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Framework/BrawlerArenaPlayerState.h"

void UPlayerHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CharacterOwner = Cast<AProyectoFinalCharacter>(GetOwningPlayer()->GetCharacter());
	DamageComponent = Cast<UDamageSystemComponent>(CharacterOwner->FindComponentByClass<UDamageSystemComponent>());
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABrawlerArenaPlayerState* PS = PC->GetPlayerState<ABrawlerArenaPlayerState>())
		{
			UpdateScoreDisplay(PS->GetScore());
			
			PS->OnScoreChanged.AddDynamic(this, &UPlayerHud::UpdateScoreDisplay);
		}
	}
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

void UPlayerHud::UpdateScoreDisplay(float NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(FMath::FloorToInt(NewScore)));
	}
}

