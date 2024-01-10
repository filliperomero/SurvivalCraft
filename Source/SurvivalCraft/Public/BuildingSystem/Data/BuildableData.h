// Copyright Fillipe Romero

#pragma once

#include "BuildableData.generated.h"

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