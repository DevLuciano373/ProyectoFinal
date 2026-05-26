#pragma once
#include "CoreMinimal.h"
#include "HealthModifierType.generated.h"

UENUM(BlueprintType)
enum class EHealthModifierType: uint8
{
	Damage UMETA(DisplayName="Daño"),
	Healing UMETA(DisplayName="Curacion")
};