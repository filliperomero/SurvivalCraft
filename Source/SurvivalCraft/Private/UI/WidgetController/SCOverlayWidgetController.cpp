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

	GetSCPC()->OnItemAddedDelegate.AddLambda(
		[this](UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName)
		{
			OnItemAddedWidgetDelegate.Broadcast(ItemIcon, ItemQuantity, ItemName);
		}
	);
}
