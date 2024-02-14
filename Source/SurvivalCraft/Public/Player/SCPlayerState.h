// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Tribe/SCTribeData.h"
#include "SCPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerXPChangedSignature, int32 /*NewTotalXPValue*/, int32 /*EarnedXP*/)

class ULevelUpInfo;

UCLASS()
class SURVIVALCRAFT_API ASCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASCPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddToLevel(int32 InLevel);
	void AddToXP(int32 InXP);
	void AddToSkillPoints(int32 InPoints);

	FOnPlayerXPChangedSignature OnXPChangedDelegate;
	FOnPlayerStatChangedSignature OnLevelChangedDelegate;
	FOnPlayerStatChangedSignature OnSkillPointsChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SkillPoints)
	int32 SkillPoints = 0;

	UPROPERTY(VisibleAnywhere, Replicated)
	bool bIsInTribe = false;

	UPROPERTY()
	FText PlayerName = FText::FromString("Player Name");

	UPROPERTY()
	FString TribeID = FString();

	UPROPERTY()
	FText TribeName = FText();

	UPROPERTY()
	ETribeRank TribeRank = ETribeRank::ETR_Member;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_SkillPoints(int32 OldSkillPoints);

public:
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }
	FORCEINLINE bool IsInTribe() const { return bIsInTribe; }
	FORCEINLINE FText GetPlayerNickname() const { return PlayerName; }
	FORCEINLINE void SetIsInTribe(const bool bInIsInTribe) { bIsInTribe = bInIsInTribe; }
	FORCEINLINE void SetTribeName(const FText& InTribeName)  { TribeName = InTribeName; }
	FORCEINLINE void SetTribeID(const FString& InTribeID)  { TribeID = InTribeID; }
};
