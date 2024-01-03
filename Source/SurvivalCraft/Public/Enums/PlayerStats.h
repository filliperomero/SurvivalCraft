#pragma once

UENUM(BlueprintType)
enum class EPlayerStats : uint8
{
	EPS_Health UMETA(DisplayName = "Health Stat"),
	EPS_Food UMETA(DisplayName = "Food Stat"),
	EPS_Water UMETA(DisplayName = "Water Stat"),
	EPS_Stamina UMETA(DisplayName = "Stamina Stat"),
};