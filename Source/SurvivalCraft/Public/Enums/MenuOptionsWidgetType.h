#pragma once

UENUM(BlueprintType)
enum class EMenuOptionsWidgetType : uint8
{
	EMO_InventoryWidget UMETA(DisplayName = "InventoryWidget"),
	EMO_TribesWidget UMETA(DisplayName = "TribesWidget"),
	EMO_SettingsWidget UMETA(DisplayName = "SettingsWidget"),
	EMO_MapWidget UMETA(DisplayName = "MapWidget")
};