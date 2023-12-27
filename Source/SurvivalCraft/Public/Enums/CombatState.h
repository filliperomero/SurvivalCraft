#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Harvesting UMETA(DisplayName = "Harvesting"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};