// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveCountdownWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API UWaveCountdownWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CountdownText;
	

	
	
	UPROPERTY()
	float Countdown = 0.f;

private:
	UFUNCTION()
	void CoundownTimer();
	
	FTimerHandle CountDownTimerHandle;
	
	UFUNCTION()
	void UpdateCountdownText();
};
