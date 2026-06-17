// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WaveCountdownWidget.h"

#include "Components/TextBlock.h"
#include "Framework/BrawlerArenaGameState.h"
#include "Kismet/GameplayStatics.h"

void UWaveCountdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ABrawlerArenaGameState* MyGameState = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState()))
	{
		// 2. Nos suscribimos al delegado
        UpdateCountdownText();
	}
}

void UWaveCountdownWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWaveCountdownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateCountdownText();
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
