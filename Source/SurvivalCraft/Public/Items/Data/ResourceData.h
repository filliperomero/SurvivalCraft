// Copyright Fillipe Romero

#pragma once

#include "ResourceData.generated.h"

class ASCDestructibleHarvestable;

USTRUCT(BlueprintType)
struct FResourceInfo : public FTableRowBase
{
	GENERATED_BODY()

	FResourceInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ResourceID = FName(NAME_None);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Quantity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EHarvestingToolType PreferredToolType = EHarvestingToolType::EHTT_Hatchet;
};

USTRUCT(BlueprintType)
struct FLargeItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FLargeItemInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASCDestructibleHarvestable> DestructibleHarvestableClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FResourceInfo> GivenItems;
};