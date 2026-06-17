// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinnerWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API UWinnerWidget : public UUserWidget
{
	GENERATED_BODY()

	protected:
		virtual void NativeOnInitialized() override;
		virtual void NativeDestruct() override;
	public:
	
		UPROPERTY(meta=(BindWidget))
		TObjectPtr<UTextBlock> WinnerPlayerText;
	
		UFUNCTION()
		void SetWinnerText(FString WinnerPlayer);
};
