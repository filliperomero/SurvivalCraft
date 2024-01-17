// Copyright Fillipe Romero

#pragma once

#include "BuildableData.generated.h"

class ASCBuildable;

UENUM(BlueprintType)
enum class EStructureDamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Metal UMETA(DisplayName = "Metal")
};

USTRUCT(BlueprintType)
struct FBuildableInfo
{
	GENERATED_BODY()

	FBuildableInfo() = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<ETraceTypeQuery> TraceChannel = TraceTypeQuery1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanPlaceOnGround = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bUseCustomOverlap = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDoFloatCheck = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanPlaceOnFoundation = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDoCeilingCheck = false;
};

// TODO: Check if we should put this in the ItemData.h
USTRUCT(BlueprintType)
struct FDamageTiers
{
	GENERATED_BODY()

	FDamageTiers() = default;

	UPROPERTY(EditDefaultsOnly)
	TArray<EStructureDamageType> DamageTypes;
};

USTRUCT(BlueprintType)
struct FBuildTableInfo :  public FTableRowBase
{
	GENERATED_BODY()

	FBuildTableInfo() = default;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASCBuildable> BuildableClass;
	
};
