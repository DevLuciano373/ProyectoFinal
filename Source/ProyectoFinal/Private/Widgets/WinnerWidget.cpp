// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WinnerWidget.h"

#include "Components/TextBlock.h"
#include "Framework/BrawlerArenaGameState.h"

class ABrawlerArenaGameState;

void UWinnerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ABrawlerArenaGameState* MyGameState = Cast<ABrawlerArenaGameState>(GetWorld()->GetGameState()))
	{
		MyGameState->OnWinnerNameChange.AddDynamic(this, &UWinnerWidget::SetWinnerText);
	}
}

void UWinnerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWinnerWidget::SetWinnerText(FString WinnerPlayer)
{
	if (WinnerPlayerText)
	{
		WinnerPlayerText->SetText(FText::FromString(WinnerPlayer));
	}
}
