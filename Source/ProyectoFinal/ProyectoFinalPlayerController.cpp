// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProyectoFinalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "ProyectoFinal.h"
#include "Components/DamageSystemComponent.h"
#include "Widgets/PlayerHud.h"
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
	
	if (PlayerHudClass)
	{
		AProyectoFinalPlayerController::SetupPlayerHud(P);
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

