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

protected:

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, FTribeInfo> TribeMap;
	
	void UpdateTribeInfoOnClients(const FTribeInfo& TribeInfo);
};
