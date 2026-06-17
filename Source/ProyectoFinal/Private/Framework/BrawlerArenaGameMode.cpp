// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/BrawlerArenaGameMode.h"

#include "Components/DamageSystemComponent.h"
#include "Utils/WarriorType.h"
#include "Framework/BrawlerArenaGameState.h"
#include "Public/Framework/BrawlerArenaPlayerState.h"

ABrawlerArenaGameMode::ABrawlerArenaGameMode()
{
	bDelayedStart = true;
}

void ABrawlerArenaGameMode::StartMatch()
{
	Super::StartMatch();
	StartNextWave();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Comenzo la partida")));
}

void ABrawlerArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	ABrawlerArenaGameState* GS = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->SetGamePhase(EMatchPhase::WaitingToStart);
		StartCountdown(CooldownWaveTime);
	}
	GetWorldTimerManager().SetTimer(MatchTimer, this, &ABrawlerArenaGameMode::StartMatch, MatchStartTimerDuration, false);

}

void ABrawlerArenaGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
	// Tomado de la IA de unreal, deberia dispararse cuando hay un error de conexion por buenas practicas
	if (!ErrorMessage.IsEmpty()) return;
	
	// Verificamos que la sala no este llena
	if (GetNumPlayers() > MaxPlayers)
	{
		ErrorMessage = TEXT("Server is full. Elimination of connection attempt.");
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Se alcanzo el numero maximo de jugadores")));
	}
}

void ABrawlerArenaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// Definimos una "Clase de jugador" para cada player participando
	AssignWarriorType(NewPlayer);
	

}

void ABrawlerArenaGameMode::AssignWarriorType(APlayerController* NewPlayer)
{
	// Validamos el PS
	ABrawlerArenaPlayerState* PS = Cast<ABrawlerArenaPlayerState>(NewPlayer->PlayerState);
	if (!PS) return;
	
	// Rellenamos el array
	if (AvailableClassesPool.Num() == 0 )
	{
		RefillWarriorClassesPool();
	}
	
	// Buscamos una clase aleatoria
	int8 RandomIndex = FMath::RandRange(0, AvailableClassesPool.Num() - 1);
	
	// Guardamos el tipo de guerrero elegido
	EWarriorType ChosenType = AvailableClassesPool[RandomIndex];
	
	// Eliminamos la clase del array de disponibles, me enetran dudas sobre cual usar, si RemoveAt o RemoveAtSwap
	AvailableClassesPool.RemoveAtSwap(RandomIndex);

	// Si existe el PS lo asignamos
	if (PS)
	{
		PS->SetWarriorType(ChosenType);
	}
}

// Se dispara cuando el array que define a los guerreros se vació
void ABrawlerArenaGameMode::RefillWarriorClassesPool()
{
	AvailableClassesPool.Empty();
	AvailableClassesPool.Add(EWarriorType::Barbarian);
	AvailableClassesPool.Add(EWarriorType::Mage);
	AvailableClassesPool.Add(EWarriorType::Archer);
	
}

void ABrawlerArenaGameMode::RespawnPlayer(AController* Controller)
{
	if (!Controller)return;
	RestartPlayer(Controller);
	APawn* PlayerPawn = Controller->GetPawn();
	if (PlayerPawn)
	{
		UDamageSystemComponent* DamageComponent = PlayerPawn->FindComponentByClass<UDamageSystemComponent>();
		if (DamageComponent)
		{
			float FullHealth = DamageComponent->MaxHealth;
			DamageComponent->HandleIncomingHeal(FullHealth, PlayerPawn);
			DamageComponent->HandleRespawn();
		}
			
	}
	
	

}

void ABrawlerArenaGameMode::HandlePlayerDeath(AController* Controller)
{
	if (!Controller) return;
	
	FTimerHandle TimerHandle;
	
	FTimerDelegate Delegate;
	Delegate.BindUObject(
		this,
		&ABrawlerArenaGameMode::RespawnPlayer,
		Controller);
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		Delegate,
		5.f,
		false);
}

void ABrawlerArenaGameMode::StartNextWave()
{
	GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);
	if (RemainingWaves > 0)
	{
		
		RemainingWaves--;
		// Obtenemos la ola actual
		int CurrentWave = EnemyWaves - RemainingWaves;
		ABrawlerArenaGameState* GS = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->SetGamePhase(EMatchPhase::WaveInProgress);
			int NumOfPlayers = GetNumPlayers();
			// Meto un numero para "ajustar" la dificultad
			GS->SetEnemiesInThisWave(CurrentWave * NumOfPlayers * 2);
			// Spawneo los enemigos
			GS->SpawnEnemiesForWave();
		}
	}
}

void ABrawlerArenaGameMode::EndWave()
{
	if (!HasAuthority())return;
	if (RemainingWaves <= 0)
	{
		// Frenamos todo, se acabo el juego
		DeclareWinner();
	}
	// Deberia tener un enum que conecte un tipo de estado del gamemode para cambiar el hud del jugador cuando
	// termine la ola actual
	
	// Curamos a los jugadores
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* PlayerPawn = PC->GetPawn();
			ABrawlerArenaPlayerState* PS = Cast<ABrawlerArenaPlayerState>(PC->PlayerState);
			UDamageSystemComponent* DamageComponent = PlayerPawn->FindComponentByClass<UDamageSystemComponent>();
			if (DamageComponent)
			{
				float FullHealth = DamageComponent->MaxHealth;
				DamageComponent->HandleIncomingHeal(FullHealth, PlayerPawn);
				if (PS)
				{
					FString PlayerName = PS->GetPlayerName();
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
					                                 FString::Printf(TEXT("Jugador %s curado"), *PlayerName));	
				}
				
			}
		}
	}
	ABrawlerArenaGameState* GS = GetGameState<ABrawlerArenaGameState>();
	
	if (GS)
	{
		GS->SetGamePhase(EMatchPhase::WaitingForNextWave);
		StartCountdown(CooldownWaveTime);
	}
	GetWorldTimerManager().SetTimer(NextWaveTimerHandle, this, &ABrawlerArenaGameMode::StartNextWave,CooldownWaveTime, false);
}

void ABrawlerArenaGameMode::DeclareWinner()
{
	// Determino el gandor desde el gamestate
	// Declarar el ganador
	// Muestro un mensaje o algo
	if (!HasAuthority())return;
	// Cambio la ui al ganador
	ABrawlerArenaGameState* GS = GetGameState<ABrawlerArenaGameState>();
	if (GS)
	{
		GS->GetWinnerPlayerState(); // Declara el ganador que emite el Gamestate
		GS->SetGamePhase(EMatchPhase::GameOver);
	}
	
	// Espero unos segundos y termino el match
	GetWorldTimerManager().SetTimer(
	TimerHandle_EndMatch,
	this,
	&ABrawlerArenaGameMode::EndMatch,
	5.0f,
	false
	);
}

void ABrawlerArenaGameMode::StartCountdown(float Duration)
{
	ABrawlerArenaGameState* GS = GetGameState<ABrawlerArenaGameState>();
	if (GS)
	{
		GS->WaveCountDown = GS->GetServerWorldTimeSeconds() + Duration;
	}
}
