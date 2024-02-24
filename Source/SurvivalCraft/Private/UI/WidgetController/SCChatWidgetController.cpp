// Copyright Fillipe Romero

#include "UI/WidgetController/SCChatWidgetController.h"
#include "Player/SCPlayerController.h"

void USCChatWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnEnterKeyPressedDelegate.AddLambda(
		[this]()
		{
			OnEnterPressedDelegate.Broadcast();
		}
	);
	
	GetSCPC()->OnReceiveChatMessageDelegate.AddLambda(
		[this](const FString& Message, const FText& TribeName, const FText& PlayerName, bool bIsGlobalMessage)
		{
			OnReceiveChatMessageWidgetDelegate.Broadcast(Message, TribeName, PlayerName, bIsGlobalMessage);
		}
	);
}

void USCChatWidgetController::SendChatMessage(const FString& Message, bool bIsGlobalMessage)
{
	GetSCPC()->ServerSendChatMessage(Message, bIsGlobalMessage);
}
