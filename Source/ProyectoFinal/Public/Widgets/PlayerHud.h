// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

class AProyectoFinalCharacter;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthText;
	
public:
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateHealth(float CurrentHealth, float MaxHealth);
	
	UPROPERTY()
	TObjectPtr<AProyectoFinalCharacter> CharacterOwner;
};
