// Copyright Fillipe Romero

#pragma once

#include "Enums/PlayerStats.h"
#include "ConsumableData.generated.h"

UENUM(BlueprintType)
enum class EConsumableDurationPolicy : uint8
{
	Overtime,
	Instant
};

USTRUCT(BlueprintType)
struct FConsumableProps
{
	GENERATED_BODY()

	FConsumableProps() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EPlayerStats StatToModify = EPlayerStats::EPS_Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EConsumableDurationPolicy DurationPolicy = EConsumableDurationPolicy::Instant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Amount = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Duration = 0.f;
};

USTRUCT(BlueprintType)
struct FConsumableItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FConsumableItemInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FConsumableProps> StatsToUpdate;
};
