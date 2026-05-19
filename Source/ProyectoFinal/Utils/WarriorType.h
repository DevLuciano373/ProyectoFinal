#pragma once
#include "CoreMinimal.h"
#include "WarriorType.generated.h"

UENUM(BlueprintType)
enum class EWarriorType: uint8
{
	None UMETA(DisplayName = "None"),
	Barbarian UMETA(DisplayName = "Barbarian"),
	Archer UMETA(DisplayName = "Archer"),
	Mage UMETA(DisplayName = "Mage"),
};
