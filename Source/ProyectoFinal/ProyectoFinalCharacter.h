// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Character_Base.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Utils/WarriorType.h"
#include "ProyectoFinalCharacter.generated.h"

class ASwordWeapon;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AProyectoFinalCharacter : public ACharacter_Base
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AProyectoFinalCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	// De aca para abajo modificamos nosotros
	
public:
	// Aca vamos a redefinir el tipo de warrior para cada cliente
	virtual void PossessedBy(AController* NewController) override;
	
	// Por ahora seteo el color de la mesh segun el tipo de EWarriorType, pero quiero cambiar la SK del character
	// cuando esta funcion se ejecute
	void UpdateWarriorMaterial(EWarriorType WarriorType);
	
	// Replicamos para el cliente cuando define el PlayerState
	virtual void OnRep_PlayerState() override;
	
	
	
protected:
	// Agregamos una accion de atacar
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;

	// Agrego la accion de curar
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* HealAction;
	
	
	// Le agrego un arma al personaje
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TSubclassOf<ASwordWeapon> SwordClass;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	FName HandSocketName = TEXT("HandGrip_R");
	
	void DoAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_DoAttack();

	UFUNCTION()
	void SpawnAndEquipWeapon();

	UFUNCTION(Server,Reliable)
	void Server_RequestEquipWeapon();
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayAttackEffects(UAnimMontage* MontageToPlay);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TObjectPtr<UAnimMontage> SwordAttackAnimMontage;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bIsAttacking = false;
	
	void DoHeal();
	virtual void BeginPlay() override;
	
public:
	UPROPERTY()
	TObjectPtr<ASwordWeapon> EquippedSword;
};


