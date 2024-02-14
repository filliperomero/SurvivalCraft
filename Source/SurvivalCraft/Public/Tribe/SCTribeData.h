// Copyright Fillipe Romero

#pragma once

#include "SCTribeData.generated.h"

class ASCPlayerController;

UENUM(BlueprintType)
enum class ETribeRank : uint8
{
	ETR_Owner UMETA(DisplayName = "Owner"),
	ETR_Admin UMETA(DisplayName = "Admin"),
	ETR_Member UMETA(DisplayName = "Member")
};

UENUM(BlueprintType)
enum class ETribeLogColor : uint8
{
	ETL_Red UMETA(DisplayName = "Tribe Log Red"),
	ETL_Yellow UMETA(DisplayName = "Tribe Log Yellow"),
	ETL_Green UMETA(DisplayName = "Tribe Log Green")
};

USTRUCT(BlueprintType)
struct FTribeMemberInfo
{
	GENERATED_BODY()

	FTribeMemberInfo() = default;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText PlayerName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PlayerLevel = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsOnline = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<ASCPlayerController> PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ETribeRank TribeRank = ETribeRank::ETR_Member;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString PlayerIDOffline = FString();
};

USTRUCT(BlueprintType)
struct FTribeLogEntry
{
	GENERATED_BODY()

	FTribeLogEntry() = default;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Time = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Day = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText LogText = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ETribeLogColor LogColor = ETribeLogColor::ETL_Green;
};

USTRUCT(BlueprintType)
struct FTribeInfo
{
	GENERATED_BODY()

	FTribeInfo() = default;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString ID = FString();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTribeMemberInfo> Members;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DayMessage = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTribeLogEntry> Logs;
};