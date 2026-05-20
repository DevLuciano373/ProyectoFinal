// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameState.h"

#include "Blueprint/UserWidget.h"

void ABrawlerArenaGameState::Multicast_OnGameEnded_Implementation(ABrawlerArenaPlayerState* WinnerPlayerState)
{
	// Bloquear los controladores del jugador local
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->IsLocalController())
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
		
		// Mostrar el widget de ganador o perdedor
		// OnShowResultsUI(WinnerPlayerState);
	}
	
}


