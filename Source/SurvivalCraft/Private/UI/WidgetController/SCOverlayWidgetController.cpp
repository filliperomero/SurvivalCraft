// Copyright Fillipe Romero

#include "UI/WidgetController/SCOverlayWidgetController.h"
#include "Player/SCPlayerController.h"

void USCOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnToggleInventoryDelegate.AddLambda(
		[this]()
		{
			OnToggleInventoryWidgetDelegate.Broadcast();
		}
	);
}
