// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameState.h"

#include "Blueprint/UserWidget.h"
#include "Framework/BrawlerArenaGameMode.h"
#include "GameFramework/GameSession.h"
#include "Net/UnrealNetwork.h"
#include "UObject/FastReferenceCollector.h"
#include "Widgets/WaveCountdownWidget.h"

// void ABrawlerArenaGameState::Multicast_OnGameEnded_Implementation(ABrawlerArenaPlayerState* WinnerPlayerState)
// {
// 	// Bloquear los controladores del jugador local
// 	APlayerController* PC = GetWorld()->GetFirstPlayerController();
// 	if (PC && PC->IsLocalController())
// 	{
// 		PC->SetInputMode(FInputModeUIOnly());
// 		PC->bShowMouseCursor = true;
// 		
// 		// Mostrar el widget de ganador o perdedor
// 		// OnShowResultsUI(WinnerPlayerState);
// 	}
// 	
// }


void ABrawlerArenaGameState::OnEnemyKilled()
{
	if (HasAuthority())
	{
		ActiveEnemies--;
		if (ActiveEnemies <= 0)
		{
			if (ABrawlerArenaGameMode* GM = GetWorld()->GetAuthGameMode<ABrawlerArenaGameMode>())
			{
				StartIntermission(GM->CooldownWaveTime);	
			}
		}
	}
}

void ABrawlerArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ActiveEnemies);
	DOREPLIFETIME(ThisClass, CurrentWave);
}

void ABrawlerArenaGameState::StartIntermission(float Duration)
{
	SecondsUntilNextWave = FMath::CeilToInt(Duration);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && CountdownWidgetClass)
	{
		CountdownWidget = CreateWidget<UWaveCountdownWidget>(PC, CountdownWidgetClass);
		if (CountdownWidget)
		{
			CountdownWidget->AddToViewport();
			CountdownWidget->UpdateTime(SecondsUntilNextWave);
		}
	}
	
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(IntermissionTimerHandle, this, &ABrawlerArenaGameState::UpdateIntermission, 1.0f, true);
	}
}

void ABrawlerArenaGameState::UpdateIntermission()
{
	SecondsUntilNextWave--;
	if (CountdownWidget)
	{
		CountdownWidget->UpdateTime(SecondsUntilNextWave);
	}
	if (SecondsUntilNextWave <= 0)
	{
		GetWorldTimerManager().ClearTimer(IntermissionTimerHandle);
		if (CountdownWidget)
		{
			CountdownWidget->RemoveFromParent();
		}
		
		if (HasAuthority())
		{
			if (ABrawlerArenaGameMode* GM = GetWorld()->GetAuthGameMode<ABrawlerArenaGameMode>())
			{
				GM->OnWaveCleared();
			}
		}
	}
	
}
