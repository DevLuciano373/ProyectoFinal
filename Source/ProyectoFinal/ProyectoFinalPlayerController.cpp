// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProyectoFinalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "ProyectoFinal.h"
#include "Widgets/PlayerHud.h"
#include "Widgets/WaveCountdownWidget.h"
#include "Widgets/WinnerWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"



void AProyectoFinalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogProyectoFinal, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}

	if (ABrawlerArenaGameState* GS = GetWorld()->GetGameState<ABrawlerArenaGameState>())
	{
		GS->OnPhaseChanged.AddDynamic(this, &AProyectoFinalPlayerController::HandlePhaseChanged);
	}
	
	OnPossessedPawnChanged.AddDynamic(this, &AProyectoFinalPlayerController::HandlePawnChanged);
	
}

void AProyectoFinalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AProyectoFinalPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AProyectoFinalPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	if (!IsLocalController())return;

}

void AProyectoFinalPlayerController::HandlePhaseChanged(EMatchPhase NewPhase)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	
	if (!IsLocalPlayerController()) return;

	switch (NewPhase)
	{
	case EMatchPhase::WaitingToStart:
		if (WaveCountDownWidgetClass)
		{
			CurrentWidget = CreateWidget<UWaveCountdownWidget>(this, WaveCountDownWidgetClass);
		}
		break;
		
	case EMatchPhase::WaveInProgress:
		if (PlayerHudClass)
		{
			CurrentWidget = CreateWidget<UPlayerHud>(this, PlayerHudClass);
			
		}
		break;
		
		case EMatchPhase::WaitingForNextWave:
		if (WaveCountDownWidgetClass)
		{
			CurrentWidget = CreateWidget<UWaveCountdownWidget>(this, WaveCountDownWidgetClass);
		}
		break;
		
	case EMatchPhase::GameOver:
		if (WinnerWidgetClass)
		{
			CurrentWidget  = CreateWidget<UWinnerWidget>(this, WinnerWidgetClass);
		}
		break;
		
	}
	
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		if (UPlayerHud* PHU = Cast<UPlayerHud>(CurrentWidget))
		{
			PHU->UpdateHealth(100, 100);	
		}
	}
}

void AProyectoFinalPlayerController::SetupPlayerHud(APawn* NewPawn)
{
	if (!NewPawn || !PlayerHudClass) return;
	
	if (!PlayerHudInstance)
	{
		PlayerHudInstance = CreateWidget<UPlayerHud>(this, PlayerHudClass);
		if (PlayerHudInstance)
		{
			PlayerHudInstance->AddToViewport();
			PlayerHudInstance->UpdateHealth(100, 100);
		}
	} else
	{
		PlayerHudInstance->RebindToComponent();
		PlayerHudInstance->UpdateHealth(100, 100);
	}
}

void AProyectoFinalPlayerController::HandlePawnChanged(APawn* PreviousPawn, APawn* NewPawn)
{
	if (!NewPawn) return;
	
	if (UPlayerHud* PlayerHud = Cast<UPlayerHud>(CurrentWidget))
	{
		PlayerHud->RebindToComponent();
		PlayerHud->UpdateHealth(100, 100);
	}
}

