// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Game/SCGameMode.h"
#include "Game/SCGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SCPlayerController.h"

void ASCGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayersList.AddUnique(NewPlayer);
}

void ASCGameMode::SendMessageToPlayers(const FString& Message, const FText& TribeName, const FText& PlayerName, bool bIsGlobalMessage, const FString& TribeID)
{
	if (bIsGlobalMessage)
	{
		for (AController* PlayerController : PlayersList)
		{
			if (IsValid(PlayerController))
			{
				if (ASCPlayerController* SCPlayerController = Cast<ASCPlayerController>(PlayerController))
				{
					SCPlayerController->ClientReceiveChatMessage(Message, TribeName, PlayerName, bIsGlobalMessage);
				}
			}
		}
	}
	else
	{
		if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			SCGameState->SendTribeMessageChat(Message, TribeID, PlayerName);
		}
	}
}
