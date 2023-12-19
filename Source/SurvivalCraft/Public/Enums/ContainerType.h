#pragma once

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	ECT_PlayerInventory UMETA(DisplayName = "Player Inventory"),
	ECT_PlayerHotbar UMETA(DisplayName = "Player Hotbar"),
	ECT_PlayerStorage UMETA(DisplayName = "Player Storage"),
	ECT_PlayerArmor UMETA(DisplayName = "Player Armour")
};