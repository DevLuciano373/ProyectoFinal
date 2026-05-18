// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuLan.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class PROYECTOFINAL_API UMenuLan : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
private:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Host;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Join;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_IPAdress;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	UFUNCTION()
	void OnButtonHostClicked();
	
	UFUNCTION()
	void OnButtonJoinClicked();
};
