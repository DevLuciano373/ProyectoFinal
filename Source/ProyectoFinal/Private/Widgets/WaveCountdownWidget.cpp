// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WaveCountdownWidget.h"

#include "Components/TextBlock.h"

void UWaveCountdownWidget::UpdateTime(int SecondsRemaining)
{
	if (CountdownText)
	{
		CountdownText->SetText(FText::AsNumber(SecondsRemaining));
	}
}
