// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Tribe/SCTribeData.h"
#include "SCGameState.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void CreateTribe(const FTribeInfo& Tribe);
	FTribeInfo* GetTribeByID(const FString& TribeID);
	bool UpdateTribeByID(const FString& TribeID, const FTribeInfo& TribeInfo);
	void AddLogToTribe(const FString& TribeID, const FTribeLogEntry& LogEntry);
	bool DemoteTribeMember(const FString& TribeID, const FString& MemberIDToDemote, const FText& PlayerWhoInitiated);
	bool PromoteTribeMember(const FString& TribeID, const FString& MemberIDToPromote, const FText& PlayerWhoInitiated);
	bool KickTribeMember(const FString& TribeID, const FString& MemberIDToKick, const FText& PlayerWhoInitiated);
	void LeaveTribe(const FString& TribeID, const FString& MemberLeavingID);
	void SetTribeMessage(const FString& TribeID, const FText& Message, const FText& PlayerWhoInitiated);
	void SendTribeMessageChat(const FString& Message, const FString& TribeID, const FText& PlayerName);
	
	static FTribeLogEntry MakeLogEntry(const FText& LogMessage, const ETribeLogColor LogColor = ETribeLogColor::ETL_Yellow);

protected:

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, FTribeInfo> TribeMap;
	
	void UpdateTribeInfoOnClients(const FTribeInfo& TribeInfo);
};
