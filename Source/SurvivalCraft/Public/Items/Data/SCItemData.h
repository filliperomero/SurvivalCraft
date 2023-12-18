// Copyright Fillipe Romero

#pragma once

#include "SCItemData.generated.h"

class ASCItemMaster;

UENUM(BlueprintType)
enum class EItemType
{
	EIT_Resource UMETA(DisplayName = "Resource"),
	EIT_Equipable UMETA(DisplayName = "Equipable"),
	EIT_Armor UMETA(DisplayName = "Armor"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Buildable UMETA(DisplayName = "Buildable")
};

UENUM(BlueprintType)
enum class EItemRarity
{
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Epic UMETA(DisplayName = "Epic"),
	EIR_Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EArmorType
{
	EAT_Helmet UMETA(DisplayName = "Helmet"),
	EAT_Chest UMETA(DisplayName = "Chest Plate"),
	EAT_Pants UMETA(DisplayName = "Pants"),
	EAT_Boots UMETA(DisplayName = "Boots")
};

USTRUCT(BlueprintType)
struct FItemInformation : public FTableRowBase
{
	GENERATED_BODY()

	FItemInformation() = default;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemQuantity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemDescription = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemCurrentHP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemMaxHP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsStackable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 StackSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemRarity ItemRarity = EItemRarity::EIR_Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType ItemType = EItemType::EIT_Resource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EArmorType ArmorType = EArmorType::EAT_Helmet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASCItemMaster> ItemClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bUseAmmo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxAmmo = 0;
};
