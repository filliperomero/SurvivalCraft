// Copyright Fillipe Romero

#include "UI/WidgetController/SCTribeWidgetController.h"
#include "Player/SCPlayerController.h"

void USCTribeWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}

void USCTribeWidgetController::CreateTribe(const FText TribeName)
{
	GetSCPC()->ServerCreateTribe(TribeName);
}
