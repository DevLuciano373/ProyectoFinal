// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WaveCountdownWidget.h"

#include "Components/TextBlock.h"
#include "Framework/BrawlerArenaGameState.h"

void UWaveCountdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ABrawlerArenaGameState* MyGameState = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState()))
	{
		// 2. Nos suscribimos al delegado
		MyGameState->OnCountdownChanged.AddDynamic(this, &UWaveCountdownWidget::UpdateCountdownText);
        UpdateCountdownText(MyGameState->GetRemainingTime());
	}
}

void UWaveCountdownWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWaveCountdownWidget::UpdateCountdownText(int32 NewTime)
{
	if (CountdownText)
	{
		FString TimeString = FString::Printf(TEXT("%d"), NewTime);
		CountdownText->SetText(FText::FromString(TimeString));
	}
	
}
