// Copyright Fillipe Romero

#include "UI/WidgetController/SCTribeWidgetController.h"
#include "Player/SCPlayerController.h"

void USCTribeWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnUpdateTribeDelegate.AddLambda(
		[this](const FTribeInfo& TribeInfo)
		{
			OnUpdateTribeWidgetDelegate.Broadcast(TribeInfo);
		}
	);
}

void USCTribeWidgetController::CreateTribe(const FText TribeName)
{
	GetSCPC()->ServerCreateTribe(TribeName);
}
