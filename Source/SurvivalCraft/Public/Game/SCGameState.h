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

protected:

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, FTribeInfo> TribeMap;
};
