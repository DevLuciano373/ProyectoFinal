// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WaveCountdownWidget.h"

#include "Components/TextBlock.h"
#include "Framework/BrawlerArenaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UWaveCountdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ABrawlerArenaGameState* MyGameState = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState()))
	{
		// 2. Nos suscribimos al delegado
        CoundownTimer();
	}
}

void UWaveCountdownWidget::NativeDestruct()
{
	Super::NativeDestruct();
}


void UWaveCountdownWidget::CoundownTimer()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(
	CountDownTimerHandle,
	this,
	&UWaveCountdownWidget::UpdateCountdownText,
	0.2f,
	true
	);
}

void UWaveCountdownWidget::UpdateCountdownText()
{
	
	if (CountdownText)
	{
		const float Timer = UGameplayStatics::GetWorldDeltaSeconds(this);
		Countdown = FMath::Clamp(Timer - Countdown, 2.f, Countdown);
		
		
		FString TimeString = FString::Printf(TEXT("%f"), Countdown);
		CountdownText->SetText(FText::FromString(TimeString));
	}
	
}
