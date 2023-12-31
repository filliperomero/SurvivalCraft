// Copyright Fillipe Romero

#pragma once

#include "CraftingData.generated.h"

UENUM(BlueprintType)
enum class ECraftingType : uint8
{
	ECFT_PlayerInventory UMETA(DisplayName = "Player Inventory"),
	ECFT_CookingPot UMETA(DisplayName = "Cooking Pot"),
	ECFT_CraftingBench UMETA(DisplayName = "Crafting Bench"),
	ECFT_Forge UMETA(DisplayName = "Forge"),
	ECFT_AdvancedWorkbench UMETA(DisplayName = "Advanced Workbench"),
	ECFT_StorageBox UMETA(DisplayName = "Storage Box")
};

UENUM(BlueprintType)
enum class ECraftingCategory : uint8
{
	ECC_Miscellaneous UMETA(DisplayName = "Miscellaneous"),
	ECC_Tools UMETA(DisplayName = "Tools"),
	ECC_Armor UMETA(DisplayName = "Armor"),
	ECC_Structures UMETA(DisplayName = "Structures")
};

USTRUCT(BlueprintType)
struct FCraftingItemInfo
{
	GENERATED_BODY()

	FCraftingItemInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemQuantity = 0;
};

USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	FCraftingRecipe() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemDescription = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RequiredLevel = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECraftingCategory ItemCategory = ECraftingCategory::ECC_Miscellaneous;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemExperience = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CraftingTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FCraftingItemInfo> RequiredItems;
};