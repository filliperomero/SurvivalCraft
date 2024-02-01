// Copyright Fillipe Romero

#pragma once

#include "EquipableData.generated.h"

class ASCEquipableItem;

UENUM(BlueprintType)
enum class EEquipableState : uint8
{
	EES_Default UMETA(DisplayName = "Default"),
	EES_Hatchet UMETA(DisplayName = "Hatchet"),
	EES_Bow UMETA(DisplayName = "Bow"),
	EES_Rock UMETA(DisplayName = "Rock"),
	EES_Rifle UMETA(DisplayName = "Rifle"), 
	EES_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EES_Spear UMETA(DisplayName = "Spear"),
	EES_Swimming UMETA(DisplayName = "Swimming")
};

UENUM(BlueprintType)
enum class EToolTier : uint8
{
	ETT_Stone UMETA(DisplayName = "Stone"),
	ETT_Iron UMETA(DisplayName = "Iron")
};

UENUM(BlueprintType)
enum class EHarvestingToolType : uint8
{
	EHTT_Pickaxe UMETA(DisplayName = "Pickaxe"),
	EHTT_Hatchet UMETA(DisplayName = "Hatchet")
};

USTRUCT(BlueprintType)
struct FEquipableItemInfo
{
	GENERATED_BODY()

	FEquipableItemInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EEquipableState EquipableState = EEquipableState::EES_Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASCEquipableItem> FirstPersonEquipClass;
	
};
