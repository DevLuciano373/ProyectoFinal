// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BrawlerArenaGameState.h"
#include "GameFramework/PlayerController.h"
#include "ProyectoFinalPlayerController.generated.h"

class UWinnerWidget;
class UWaveCountdownWidget;
class UPlayerHud;
class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AProyectoFinalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:


	
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

	// Para el hud
	virtual void AcknowledgePossession(class APawn* P) override;
	
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UPlayerHud> PlayerHudClass;
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UWaveCountdownWidget> WaveCountDownWidgetClass;
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UWinnerWidget> WinnerWidgetClass;
	
	UFUNCTION()
	void HandlePhaseChanged(EMatchPhase NewPhase);
	
	
	


	
private:
	UPROPERTY()
	TObjectPtr<UPlayerHud> PlayerHudInstance;
	
	UPROPERTY()
	TObjectPtr<UWaveCountdownWidget> WaveCountdownInstance;
	
	UPROPERTY()
	TObjectPtr<UWinnerWidget> WinnerWidgetInstance;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;
	
	UFUNCTION()
	void SetupPlayerHud(APawn* NewPawn);	
	
	UFUNCTION()
	void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);
	
};

