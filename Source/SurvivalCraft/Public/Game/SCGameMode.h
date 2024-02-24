// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SCGameMode.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SendMessageToPlayers(const FString& Message, const FText& TribeName, const FText& PlayerName, bool bIsGlobalMessage, const FString& TribeID);

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	UPROPERTY()
	TArray<AController*> PlayersList;
};
