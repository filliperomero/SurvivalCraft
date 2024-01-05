// Copyright Fillipe Romero

#include "UI/WidgetController/SCOverlayWidgetController.h"
#include "Player/SCPlayerController.h"
#include "Player/SCPlayerState.h"

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

	GetSCPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 NewXP, int32 EarnedXP)
		{
			OnXPChangedDelegate.Broadcast(static_cast<float>(EarnedXP));
		}
	);

	GetSCPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(static_cast<float>(NewLevel));
		}
	);
}
