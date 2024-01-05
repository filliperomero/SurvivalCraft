// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FSCLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointAward = 0;
};

UCLASS()
class SURVIVALCRAFT_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FSCLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP) const;
};
