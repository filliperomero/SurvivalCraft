// Copyright Fillipe Romero

#include "UI/WidgetController/SCWidgetController.h"
#include "Player/SCPlayerController.h"

void USCWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
}

void USCWidgetController::BroadcastInitialValues()
{
}

void USCWidgetController::BindCallbacksToDependencies()
{
}

ASCPlayerController* USCWidgetController::GetSCPC()
{
	if (SCPlayerController == nullptr)
	{
		SCPlayerController = Cast<ASCPlayerController>(PlayerController);
	}

	return SCPlayerController;
}
